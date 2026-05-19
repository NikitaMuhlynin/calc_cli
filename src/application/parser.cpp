#include "calc_cli/application/parser.h"
#include "calc_cli/application/context.h"
#include "calc_cli/application/logger.h"

#include <stdexcept>
#include <string>
#include <unordered_map>

namespace calc_cli {

ApplicationContext CommandLineParser::parse(const std::string& request) {
    try {
        const auto data = nlohmann::json::parse(request);

        Logger::instance().info("Received input: {}", request);

        if (data.contains("help") && data.at("help").get<bool>())
            return ApplicationContext{
                0, 0, Operation::Add, 0, 1, 0
            };
        
        return parseRequest(data);
    } catch(const nlohmann::json::exception&) {
        Logger::instance().warn("Invalid JSON input");
        throw std::invalid_argument("Error: invalid JSON input");
    }
}

ApplicationContext CommandLineParser::parseRequest(const nlohmann::json& data) {
    if (!data.contains("left"))
        throw std::invalid_argument("Error: field 'left' is required");
    
    if (!data.contains("operation")) {
        Logger::instance().warn("Missing required field: operation"); 
        throw std::invalid_argument("Error: field 'operation' is required");
    }
    
    ApplicationContext request;
    request.help_requested = 0;
    request.left = data.at("left").get<long long>();

    const auto value = data.at("operation").get<std::string>();
    request.operation = OperationTraits::fromString(value);
   
    if (request.operation != Operation::Factorial) {
        if (!data.contains("right")) {
            request.parse_status = 1;
            throw std::invalid_argument("Error: field 'right' is required");
        }
        
        request.right = data.at("right").get<long long>();
    }
    
    request.parse_status = 0;
    return request;
}

}