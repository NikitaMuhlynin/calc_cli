#include "../include/calc_cli/parser.h"
#include "../include/calc_cli/context.h"
#include "../include/calc_cli/logger.h"

#include <stdexcept>
#include <string>
#include <unordered_map>

namespace calc_cli {

ApplicationContext CommandLineParser::parse(int argc, char** argv) {
    if (argc < 2) 
        throw std::invalid_argument("Error: JSON input is missing");

    try {
        const auto data = nlohmann::json::parse(argv[1]);

        Logger::instance()->info("Received input: {}", argv[1]);

        if (data.contains("help") && data.at("help").get<bool>())
            return ApplicationContext{
                0, 0, Operation::Add, 0, 1
            };
        
        return parseRequest(data);
    } catch(const nlohmann::json::exception&) {
        Logger::instance()->warn("Invalid JSON input");
        throw std::invalid_argument("Error: invalid JSON input");
    }
}

ApplicationContext CommandLineParser::parseRequest(const nlohmann::json& data) {
    if (!data.contains("left"))
        throw std::invalid_argument("Error: field 'left' is required");
    
    if (!data.contains("operation")) {
        Logger::instance()->warn("Missing required field: operation"); 
        throw std::invalid_argument("Error: field 'operation' is required");
    }
    
    ApplicationContext request;
    request.help_requested = 0;
    request.left = data.at("left").get<long long>();
    request.operation = parseOperation(data.at("operation").get<std::string>());

    if (request.operation != Operation::Factorial) {
        if (!data.contains("right"))
            throw std::invalid_argument("Error: field 'right' is required");
        
        request.right = data.at("right").get<long long>();
    }
    
    return request;
}

Operation CommandLineParser::parseOperation(const std::string& value) {
    static const std::unordered_map<std::string, Operation> operations = {
        {"add", Operation::Add},
        {"subtract", Operation::Subtract},
        {"multiply", Operation::Multiply},
        {"divide", Operation::Divide},
        {"power", Operation::Power},
        {"factorial", Operation::Factorial}
    };

    const auto it = operations.find(value);
    if (it == operations.end()) {
        throw std::invalid_argument("Error: unknown operation");
    }

    return it->second;
}

}