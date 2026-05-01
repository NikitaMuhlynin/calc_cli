#pragma once

#include <ostream>

#include "calc_cli/calculator.h"
#include "calc_cli/context.h"
#include "calc_cli/parser.h"
#include "calc_cli/printer.h"
#include "calc_cli/logger.h"

namespace calc_cli {

class Runner {
public:
    Runner(std::ostream& out, std::ostream& err);
    ~Runner() = default;

    Runner(const Runner&) = default;
    Runner& operator=(const Runner&) = default;
    
    Runner(Runner&&) = default;
    Runner& operator=(const Runner&&) = default;

    int run(int argc, char** argv);

private:
    CommandLineParser parser_;
    Calculator calculator_;
    Printer printer_;
    std::ostream& out_;
    std::ostream& err_;
};

}