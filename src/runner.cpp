#include "calc_cli/runner.h"

#include <iostream>

namespace calc_cli {

    Runner::Runner(std::ostream& out, std::ostream& err)
        : parser_{}, calculator_{}, printer_{}, out_{out}, err_{err} {}

    int Runner::run(int argc, char** argv) {
        try {
            Logger::instance()->info("Program started");

            ApplicationContext result = parser_.parse(argc, argv);

            if (result.help_requested == false) {
                calculator_.calculate(result);

                Logger::instance()->info("Calculation completed: {}", result.result);

                printer_.printResult(out_, result.result);
                return 0;
            }

            Logger::instance()->info("Help requested");
            
            printer_.printHelp(out_, argv[0]);
            return 0;
        } catch(const std::exception& e) {
            Logger::instance()->error("Execution failed: {}", e.what());

            printer_.printError(err_, e.what());
            return 1;
        }
    }

}