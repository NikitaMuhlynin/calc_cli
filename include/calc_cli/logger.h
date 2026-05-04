#pragma once

#include <string_view>

namespace calc_cli {

class Logger {
public:
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    Logger(Logger&&) noexcept = delete;
    Logger& operator=(Logger&&) noexcept = delete;

    static Logger& instance();

    void trace(std::string_view message) const;
    void debug(std::string_view message) const;
    void info(std::string_view message) const;
    void warn(std::string_view message) const;
    void error(std::string_view message) const;
    void critical(std::string_view message) const;

private:
    Logger() = default;
    ~Logger() = default;

};

}