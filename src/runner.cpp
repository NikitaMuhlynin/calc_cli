#include "calc_cli/runner.h"

#include "calc_cli/calculator.h"
#include "calc_cli/context.h"
#include "calc_cli/parser.h"
#include "calc_cli/printer.h"
#include "calc_cli/logger.h"


namespace calc_cli {

    int Runner::run(int argc, char** argv) {
        try {
            Logger::instance()->info("Program started");

            ParseResult result = CommandLineParser::parse(argc, argv);

            if (result.help_requested == false) {
                long long calc_result = Calculator::calculate(*result.request);

                Logger::instance()->info("Calculation completed: {}", calc_result);

                Printer::print_result(calc_result);
                return 0;
            }

            Logger::instance()->info("Help requested");
            
            Printer::print_help(argv[0]);
            return 0;
        } catch(const std::exception& e) {
            Logger::instance()->error("Execution failed: {}", e.what());

            Printer::print_error(e.what());
            return 1;
        }
    }

}