#include <nlohmann/json.hpp>

#include "context.h"

namespace calc_cli {

class CommandLineParser {
public:
    CommandLineParser() = default;
    ~CommandLineParser() = default;

    CommandLineParser(const CommandLineParser&) = default;
    CommandLineParser& operator=(const CommandLineParser&) = default;

    CommandLineParser(CommandLineParser&&) = default;
    CommandLineParser& operator=(CommandLineParser&&) = default;    

    ApplicationContext parse(int argc, char** argv);

private:
    ApplicationContext parseRequest(const nlohmann::json& data);
    Operation parseOperation(const std::string& value);
};

}