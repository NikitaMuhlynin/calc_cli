#include <nlohmann/json.hpp>

#include "context.h"

namespace calc_cli {

class CommandLineParser {
public:
    CommandLineParser() = delete;

    static ParseResult parse(int argc, char** argv);

private:
    static CalculationRequest parseRequest(const nlohmann::json& data);
    static Operation parseOperation(const std::string& value);
};

}