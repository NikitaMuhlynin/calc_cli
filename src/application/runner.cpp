#include "calc_cli/application/runner.h"

#include <string>
#include <thread>
#include <csignal>
#include <iostream>
#include <stdexcept>
#include <pthread.h>

namespace calc_cli {

Runner::Runner(std::ostream& out, std::ostream& err, const std::string& connectionString)
    : running_{true},
    parser_{}, 
    calculator_{}, 
    printer_{}, 
    out_{out}, 
    err_{err},
    storage_{connectionString},
    cache_{storage_.loadAll()} {}

void Runner::run() {
    Logger::instance().info("Program started");
    Logger::instance().info("Memory cache warmed up: {}", cache_.size());

    sigset_t sigset;
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGINT);
    sigaddset(&sigset, SIGTERM);
    sigaddset(&sigset, SIGHUP);
    pthread_sigmask(SIG_BLOCK, &sigset, nullptr);

    user_thread_ = std::thread(&Runner::user_thread, this);
    signal_thread_ = std::thread(&Runner::signal_thread, this);
    
    if (user_thread_.joinable()) {
        user_thread_.join();
    }

    running_.store(false);

    if (signal_thread_.joinable()) {
        pthread_kill(signal_thread_.native_handle(), SIGTERM);
        signal_thread_.join();
    }
}

void Runner::signal_thread() {
    sigset_t sigset;
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGINT);
    sigaddset(&sigset, SIGTERM);
    sigaddset(&sigset, SIGHUP);

    int sig;
    while(true) {
        if (sigwait(&sigset, &sig) == 0) {
            switch(sig) {
                case SIGINT:
                    std::cout << "SIGINT received: stopping gracefully\n";
                    running_.store(false);
                    return;
                case SIGTERM:
                    std::cout << "SIGTERM received: stopping immediately\n";
                    running_.store(false);
                    return;
                case SIGHUP:
                    std::cout << "SIGHUP received: triggering systemd restart\n";
                    running_.store(false);
                    return;
                default:
                    std::cout << "Unknown signal: " << sig << "\n";
            }
        }
    } 
}

void Runner::user_thread() {
    while(running_.load()) {
        try {
            std::string input;
            std::cout << "Enter JSON request: ";
            if (!std::getline(std::cin, input))
                throw std::runtime_error("Error: empty JSON request");

            ApplicationContext context = parser_.parse(input);

            if (context.help_requested != 0) {
                Logger::instance().info("Help requested");
                printer_.printHelp(out_);
                continue;
            }

            std::string key = OperationTraits::makeNormalizedKey(context);

            std::optional<ApplicationContext> cachedContext = cache_.find(key);

            if (cachedContext.has_value()) {
                Logger::instance().info("Result found in memory cache");

                printer_.printResult(out_, cachedContext.value().result);
                continue;
            }

            Logger::instance().info("Result not found in memory cache");

            calculator_.calculate(context);

            cache_.save(key, context);
            storage_.save(context);

            Logger::instance().info(
                "Calculation completed with status: {}",
                context.parse_status
            );

            printer_.printResult(out_, context.result);
        } catch(const std::exception& e) {
            Logger::instance().error("Execution failed: {}", e.what());

            printer_.printError(err_, e.what());
        }
    }
}

Runner::~Runner() {
    if (user_thread_.joinable())
        user_thread_.join();
    
    if (signal_thread_.joinable())
        signal_thread_.join();
}

}