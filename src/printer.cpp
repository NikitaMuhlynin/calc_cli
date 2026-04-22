#include "calc_cli/printer.h"

#include <stdio.h>

namespace calc_cli {

    static const char* operation_status_to_text(safe_math::ErrorCode status) {
        switch (status) {
        case safe_math::ERROR_OK:
            return "ok";

        case safe_math::ERROR_OVERFLOW:
            return "integer overflow";

        case safe_math::ERROR_DIVISION_BY_ZERO:
            return "division by zero";

        case safe_math::ERROR_NEGATIVE_ARGUMENT:
            return "negative argument";

        case safe_math::ERROR_NEGATIVE_EXPONENT:
            return "negative exponent";

        default:
            return "unknown error";
        }
    }

    void print_help(const char* program_name) {
        printf("Usage:\n");
        printf("  %s --first <int> --second <int> --operation <op>\n", program_name);
        printf("\n");
        printf("Options:\n");
        printf("  -h, --help              Show this help\n");
        printf("  -a, --first <int>       First integer number\n");
        printf("  -b, --second <int>      Second integer number\n");
        printf("  -o, --operation <op>    Operation: add, sub, mul, div, pow, fact\n");
        printf("\n");
        printf("Notes:\n");
        printf("  For factorial, --first is used and --second is accepted but ignored.\n");
        printf("\n");
        printf("Examples:\n");
        printf("  %s -a 2 -b 3 -o add\n", program_name);
        printf("  %s -a 10 -b 2 -o div\n", program_name);
        printf("  %s -a 2 -b 10 -o pow\n", program_name);
        printf("  %s -a 5 -b 0 -o fact\n", program_name);
    }

    void print_result(const ApplicationContext* context) {
        if (context->operation_status == safe_math::ERROR_OK) {
            printf("Result: %lld\n", context->result);
            return;
        }

        printf("Error: %s\n", operation_status_to_text(context->operation_status));
    }

    void print_parse_error(const char* program_name) {
        printf("Invalid command line arguments.\n\n");
        print_help(program_name);
    }

}