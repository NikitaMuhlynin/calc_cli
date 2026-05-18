#pragma once

#include <ostream>

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
    ~Runner() = default;

    Runner(const Runner&) = default;
    Runner& operator=(const Runner&) = default;
    
    Runner(Runner&&) noexcept = default;
    Runner& operator=(const Runner&&) noexcept = default;

    int run(int argc, char** argv);

private:
    CommandLineParser parser_;
    Calculator calculator_;
    Printer printer_;
    Storage storage_;
    MemoryCache cache_;
    std::ostream& out_;
    std::ostream& err_;
};

}