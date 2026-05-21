#include "calc_cli/application/runner.h"

#include <string>
#include <thread>
#include <csignal>
#include <iostream>
#include <stdexcept>
#include <pthread.h>

namespace calc_cli {

Runner::Runner(const std::string& connectionString, std::uint16_t port)
    : running_{true},
    io_context_{},
    acceptor_{io_context_},
    port_{port},
    server_thread_{},
    signal_thread_{},
    parser_{}, 
    calculator_{}, 
    printer_{}, 
    storage_{connectionString},
    cache_{storage_.loadAll()} {}

Runner::~Runner() {
    request_stop();

    if (signal_thread_.joinable()) {
        pthread_kill(signal_thread_.native_handle(), SIGTERM);
    }

    join_threads();
}

void Runner::run() {
    Logger::instance().info("Program started");
    Logger::instance().info("Memory cache warmed up: {}", cache_.size());

    sigset_t sigset;
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGINT);
    sigaddset(&sigset, SIGTERM);
    sigaddset(&sigset, SIGHUP);

    pthread_sigmask(SIG_BLOCK, &sigset, nullptr);

    signal_thread_ = std::thread(&Runner::signal_thread, this);
    server_thread_ = std::thread(&Runner::server_thread, this);
    
    if (server_thread_.joinable()) {
        server_thread_.join();
    }

    request_stop();

    if (signal_thread_.joinable()) {
        pthread_kill(signal_thread_.native_handle(), SIGTERM);
        signal_thread_.join();
    }
}

void Runner::request_stop() {
    const bool wasRunning = running_.exchange(false);

    if (!wasRunning)
        return;

    boost::asio::post(
        io_context_,
        [this]() {
            boost::system::error_code ignoredError;

            acceptor_.close(ignoredError);
            io_context_.stop();
        }
    );
}

void Runner::join_threads() {
    if (signal_thread_.joinable())
        signal_thread_.join();
    
    if (server_thread_.joinable())
        server_thread_.join();
}

void Runner::signal_thread() {
    sigset_t sigset;
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGINT);
    sigaddset(&sigset, SIGTERM);
    sigaddset(&sigset, SIGHUP);

    int sig = 0;

    while(running_.load()) {
        if (sigwait(&sigset, &sig) == 0) {
            switch(sig) {
                case SIGINT:
                Logger::instance().info("SIGINT received: stopping");
                request_stop();
                return;

            case SIGTERM:
                Logger::instance().info("SIGTERM received: stopping");
                request_stop();
                return;

            case SIGHUP:
                Logger::instance().info("SIGHUP received: reloading cache");
                cache_.load(storage_.loadAll());
                break;

            default:
                Logger::instance().warn("Unknown signal received: {}", sig);
                break;
            }
        }
    } 
}

void Runner::server_thread() {
    try {
        boost::asio::ip::tcp::endpoint endpoint(
            boost::asio::ip::tcp::v4(),
            port_
        );

        boost::system::error_code error;

        acceptor_.open(endpoint.protocol(), error);
        if (error) {
            throw std::runtime_error("Failed to open acceptor: " + error.message());
        }

        acceptor_.set_option(
            boost::asio::ip::tcp::acceptor::reuse_address(true),
            error
        );
        if (error) {
            throw std::runtime_error("Failed to set reuse_address: " + error.message());
        }

        acceptor_.bind(endpoint, error);
        if (error) {
            throw std::runtime_error("Failed to bind port: " + error.message());
        }

        acceptor_.listen(
            boost::asio::socket_base::max_listen_connections,
            error
        );
        if (error) {
            throw std::runtime_error("Failed to listen: " + error.message());
        }

        Logger::instance().info("TCP server started on port {}", port_);

        start_accept();

        io_context_.run();

        Logger::instance().info("TCP server stopped");
    } catch(const std::exception& e) {
        Logger::instance().error("Server thread failed: {}", e.what());
        request_stop();
    }
}

void Runner::start_accept() {
    if (!running_.load()) {
        return;
    }

    auto socket = std::make_shared<boost::asio::ip::tcp::socket>(io_context_);

    acceptor_.async_accept(
        *socket,
        [this, socket](const boost::system::error_code& error) {
            if (!running_.load()) {
                return;
            }

            if (error) {
                Logger::instance().error("Accept failed: {}", error.message());

                if (running_.load()) {
                    start_accept();
                }

                return;
            }

            Logger::instance().info("Client connected");

            handle_client(socket);
        }
    );
}

void Runner::handle_client(
    std::shared_ptr<boost::asio::ip::tcp::socket> socket
) {
    auto buffer = std::make_shared<boost::asio::streambuf>();

    boost::asio::async_read_until(
        *socket,
        *buffer,
        '\n',
        [this, socket, buffer](
            const boost::system::error_code& error,
            std::size_t
        ) {
            std::string response;

            if (error) {
                response = printer_.printError(
                    "Failed to read request: " + error.message()
                );
            } else {
                std::istream inputStream(buffer.get());

                std::string request;
                std::getline(inputStream, request);

                Logger::instance().info("Recieved request: {}", request);

                try {
                    response = process_request(request);
                } catch (const std::exception& e) {
                    Logger::instance().error(
                        "Request processing failed: {}",
                        e.what()
                    );

                    response = printer_.printError(e.what());
                }
            }

            response += '\n';

            auto responcePtr = std::make_shared<std::string>(std::move(response));

            boost::asio::async_write(
                *socket,
                boost::asio::buffer(*responcePtr),
                [this, socket, responcePtr](
                    const boost::system::error_code& writeError,
                    std::size_t
                ) {
                    if (writeError) {
                        Logger::instance().error(
                            "Failed to write response: {}",
                            writeError.message()
                        );
                    }

                    if (running_.load()) {
                        start_accept();
                    }
                }
            );
        }
    );
}

std::string Runner::process_request(const std::string& request) {
    ApplicationContext context = parser_.parse(request);

    if (context.help_requested != 0) {
        Logger::instance().info("Help requested");
        return printer_.printHelp();
    }

    std::string key = OperationTraits::makeNormalizedKey(context);

    std::optional<ApplicationContext> cachedContext = cache_.find(key);

    if (cachedContext.has_value()) {
        Logger::instance().info("Result found in memory cache");
        return printer_.printResult(cachedContext.value().result);
    }

    Logger::instance().info("Result not found in memory cache");

    calculator_.calculate(context);

    cache_.save(key, context);
    storage_.save(context);

    Logger::instance().info(
        "Calculation completed with status: {}",
        context.parse_status
    );

    return printer_.printResult(context.result);
}


}