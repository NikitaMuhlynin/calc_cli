#pragma once

#include <ostream>
#include <atomic>
#include <thread>

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
    Runner(std::ostream& out, std::ostream& err, const std::string& connectionString);
    ~Runner();

    Runner(const Runner&) = delete;
    Runner& operator=(const Runner&) = delete;
    
    Runner(Runner&&) noexcept = delete;
    Runner& operator=(const Runner&&) noexcept = delete;

    void run();

private:
    void signal_thread();
    void user_thread();

private:
    std::thread user_thread_;
    std::thread signal_thread_;
    std::atomic<bool> running_;

    CommandLineParser parser_;
    Calculator calculator_;
    Printer printer_;
    Storage storage_;
    MemoryCache cache_;
    std::ostream& out_;
    std::ostream& err_;
};

}