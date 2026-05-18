#include "calc_cli/application/runner.h"

#include <iostream>

namespace calc_cli {

Runner::Runner(std::ostream& out, std::ostream& err, const std::string& connectionString)
    : parser_{}, 
    calculator_{}, 
    printer_{}, 
    out_{out}, 
    err_{err},
    storage_{connectionString},
    cache_{storage_.loadAll()} {}

int Runner::run(int argc, char** argv) {
    try {
        Logger::instance().info("Program started");
        Logger::instance().info("Memory cache warmed up: {}", cache_.size());
        
        ApplicationContext context = parser_.parse(argc, argv);

        if (context.help_requested != 0) {
            Logger::instance().info("Help requested");
            printer_.printHelp(out_, argv[0]);
            return 0;
        }

        std::string key = OperationTraits::makeNormalizedKey(context);

        std::optional<ApplicationContext> cachedContext = cache_.find(key);

        if (cachedContext.has_value()) {
            Logger::instance().info("Result found in memory cache");

            printer_.printResult(out_, cachedContext.value().result);

            return 0;
        }

        Logger::instance().info("Result not found in memory cache");

        calculator_.calculate(context);

        cache_.save(key, context);
        storage_.save(context);

        Logger::instance().info(
            "Calculation completed with status: {}",
            context.parse_status
        );

        printer_.printResult(out_, context.result);

        return 0;
    } catch(const std::exception& e) {
        Logger::instance().error("Execution failed: {}", e.what());

        printer_.printError(err_, e.what());
        return 1;
    }
}

}