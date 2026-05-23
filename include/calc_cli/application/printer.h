#pragma once

#include <ostream>
#include <string>

namespace calc_cli {

class Printer {
public:
    Printer() = default;
    ~Printer() = default;

    Printer(const Printer&) = default;
    Printer& operator=(const Printer&) = default;

    Printer(Printer&&) noexcept = default;
    Printer& operator=(Printer&&) noexcept = default;

    std::string printResult(long long result) const;
    std::string printHelp() const;
    std::string printError(const std::string& message) const;
};

}