#include "calc_cli/parser.h"

#include <errno.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>

namespace calc_cli {

    static int parse_long_long(const char* text, long long* value) {
        char* end = 0;
        long long parsed = 0;

        errno = 0;
        parsed = strtoll(text, &end, 10);

        if (errno == ERANGE || end == text || *end != '\0')
            return 0;
        
        *value = parsed;
        return 1;
    }

    static Operation parse_operation(const char* text) {
        if (strcmp(text, "add") == 0 || strcmp(text, "+") == 0)
            return OPERATION_ADD;
        
        if (strcmp(text, "sub") == 0 || strcmp(text, "-") == 0)
            return OPERATION_SUBTRACT;
        
        if (strcmp(text, "mul") == 0 || strcmp(text, "*") == 0)
            return OPERATION_MULTIPLY;
        
        if (strcmp(text, "div") == 0 || strcmp(text, "/") == 0)
            return OPERATION_DIVIDE;
        
        if (strcmp(text, "pow") == 0 || strcmp(text, "power") == 0)
            return OPERATION_POWER;
        
        if (strcmp(text, "fact") == 0 || strcmp(text, "factorial") == 0)
            return OPERATION_FACTORIAL;
        
        return OPERATION_NONE;
    }

    int parse_arguments(int argc, char** argv, ApplicationContext* context) {
        int option = 0;
        int has_first_number = 0;
        int has_second_number = 0;
        int has_operation = 0;

        static struct option long_options[] = {
            {"help", no_argument, 0, 'h'},
            {"first", required_argument, 0, 'a'},
            {"second", required_argument, 0, 'b'},
            {"operation", required_argument, 0, 'o'},
            {0, 0, 0, 0}
        };

        while ((option = getopt_long(argc, argv, "ha:b:o:", long_options, 0)) != -1) {
            switch (option)
            {
            case 'h':
                context->help_requested = 1;
                context->parse_status = 1;
                return 1;
            
            case 'a':
                if (!parse_long_long(optarg, &context->first_number)) {
                    context->parse_status = 0;
                    return 0;
                }

                has_first_number = 1;
                break;
            
            case 'b':
                if (!parse_long_long(optarg, &context->second_number)) {
                    context->parse_status = 0;
                    return 0;
                }
                
                has_second_number = 1;
                break;

            case 'o':
                context->operation = parse_operation(optarg);

                if (context->operation == OPERATION_NONE) {
                    context->parse_status = 0;
                    return 0;
                }

                has_operation = 1;
                break;
            
            default:
                context->parse_status = 0
                return 0;
            }
        }

        if (optind < argc) {
            context->parse_status = 0;
            return 0;
        }

        if (!has_first_number || !has_second_number || !has_operation) {
            context->parse_status = 0;
            return 0;
        }
        
        context->parse_status = 1;
        return 1;
    }
}