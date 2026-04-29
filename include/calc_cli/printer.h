#pragma once

#include "calc_cli/context.h"

#include <string>

namespace calc_cli {

class Printer{
public:
    Printer() = delete;

    static void print_help(const char* program_name);
    static void print_result(long long result);
    static void print_error(const std::string& message);
};

}

