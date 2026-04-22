#include "calc_cli/runner.h"

#include "calc_cli/calculator.h"
#include "calc_cli/context.h"
#include "calc_cli/parser.h"
#include "calc_cli/printer.h"

namespace calc_cli {

    static void initialize_context(ApplicationContext* context) {
        context->first_number = 0;
        context->second_number = 0;
        context->operation = OPERATION_NONE;
        context->result = 0;
        context->operation_status = safe_math::ERROR_OK;
        context->help_requested = 0;
        context->parse_status = 0;
    }

    int run(int argc, char** argv) {
        ApplicationContext context;

        initialize_context(&context);

        if (!parse_arguments(argc, argv, &context)) {
            print_parse_error(argv[0]);
            return 2;
        }

        if (context.help_requested) {
            print_help(argv[0]);
            return 0;
        }

        calculate(&context);
        print_result(&context);

        if (context.operation_status != safe_math::ERROR_OK)
            return 1;
        
        return 0;
    }

}