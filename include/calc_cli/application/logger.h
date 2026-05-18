#pragma once

#include <string_view>
#include <format>

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

    template <typename T>
    void trace(std::format_string<T> format, T&& value) const {
        trace(std::format(format, std::forward<T>(value)));
    }

    template <typename T>
    void debug(std::format_string<T> format, T&& value) const {
        debug(std::format(format, std::forward<T>(value)));
    }

    template <typename T>
    void info(std::format_string<T> format, T&& value) const {
        info(std::format(format, std::forward<T>(value)));
    }

    template <typename T>
    void warn(std::format_string<T> format, T&& value) const {
        warn(std::format(format, std::forward<T>(value)));
    }

    template <typename T>
    void error(std::format_string<T> format, T&& value) const {
        error(std::format(format, std::forward<T>(value)));
    }

    template <typename T>
    void critical(std::format_string<T> format, T&& value) const {
        critical(std::format(format, std::forward<T>(value)));
    }

private:
    Logger() = default;
    ~Logger() = default;

};

}