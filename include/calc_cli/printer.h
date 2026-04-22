#pragma once

#include "calc_cli/context.h"

namespace calc_cli {

    void print_help(const char* program_name);
    void print_result(const ApplicationContext* context);
    void print_parse_error(const char* program_name);

}