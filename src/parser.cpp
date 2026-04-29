#include "../include/calc_cli/parser.h"
#include "../include/calc_cli/context.h"
#include "../include/calc_cli/logger.h"

#include <stdexcept>
#include <string>

namespace calc_cli {

ParseResult CommandLineParser::parse(int argc, char** argv) {
    if (argc < 2) 
        throw std::invalid_argument("Error: JSON input is missing");

    try {
        const auto data = nlohmann::json::parse(argv[1]);

        Logger::instance()->info("Received input: {}", argv[1]);

        if (data.contains("help") && data.at("help").get<bool>())
            return calc_cli::ParseResult{true, std::nullopt};

        return ParseResult(false, parseRequest(data));

    } catch(const nlohmann::json::exception&) {
        Logger::instance()->warn("Invalid JSON input");
        throw std::invalid_argument("Error: invalid JSON input");
    }
}

CalculationRequest CommandLineParser::parseRequest(const nlohmann::json& data) {
    if (!data.contains("left"))
        throw std::invalid_argument("Error: field 'left' is required");
    
    if (!data.contains("operation")) {
        Logger::instance()->warn("Missing required field: operation"); 
        throw std::invalid_argument("Error: field 'operation' is required");
    }
    
    CalculationRequest request;
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
    if (value == "add") {
        return Operation::Add;
    }
    if (value == "subtract") {
        return Operation::Subtract;
    }
    if (value == "multiply") {
        return Operation::Multiply;
    }
    if (value == "divide") {
        return Operation::Divide;
    }
    if (value == "power") {
        return Operation::Power;
    }
    if (value == "factorial") {
        return Operation::Factorial;
    }

    throw std::invalid_argument("Error: unknown operation");
}

}