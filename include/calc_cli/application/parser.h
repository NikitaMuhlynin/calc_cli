#pragma once

#include <string>
#include <nlohmann/json.hpp>

#include "calc_cli/application/context.h"

namespace calc_cli {

class CommandLineParser {
public:
    CommandLineParser() = default;
    ~CommandLineParser() = default;

    CommandLineParser(const CommandLineParser&) = default;
    CommandLineParser& operator=(const CommandLineParser&) = default;

    CommandLineParser(CommandLineParser&&) noexcept = default;
    CommandLineParser& operator=(CommandLineParser&&) noexcept = default;    

    ApplicationContext parse(const std::string& request);

private:
    ApplicationContext parseRequest(const nlohmann::json& data);
};

}