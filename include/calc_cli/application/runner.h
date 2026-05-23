#pragma once

#include <atomic>
#include <thread>
#include <cstdint>
#include <string>
#include <memory>

#include <boost/asio.hpp>

#include "calc_cli/application/calculator.h"
#include "calc_cli/application/parser.h"
#include "calc_cli/application/printer.h"
#include "calc_cli/application/logger.h"
#include "calc_cli/application/context.h"
#include "calc_cli/cache_service/memory_cache.h"
#include "calc_cli/cache_service/storage.h"

namespace calc_cli {

class Runner {
public:
    Runner(
        const std::string& connectionString,
        std::uint16_t port
    );

    ~Runner();

    Runner(const Runner&) = delete;
    Runner& operator=(const Runner&) = delete;
    
    Runner(Runner&&) noexcept = delete;
    Runner& operator=(const Runner&&) noexcept = delete;

    void run();
    void stop();

private:
    void signal_thread();
    void server_thread();

    void start_accept();

    void handle_client(
        std::shared_ptr<boost::asio::ip::tcp::socket> socket
    );

    std::string process_request(const std::string& request);

    void request_stop();
    void join_threads();

private:
    std::atomic<bool> running_;

    boost::asio::io_context io_context_;
    boost::asio::ip::tcp::acceptor acceptor_;
    std::uint16_t port_;

    std::thread server_thread_;
    std::thread signal_thread_;

    CommandLineParser parser_;
    Calculator calculator_;
    Printer printer_;

    Storage storage_;
    MemoryCache cache_;
};

}