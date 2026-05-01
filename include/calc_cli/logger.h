#pragma once

#include <memory>
#include <spdlog/logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace calc_cli {

class Logger {
public:
    Logger() = delete;
    ~Logger() = default;

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    Logger(Logger&&) = delete;
    Logger& operator=(Logger&&) = delete;

    static std::shared_ptr<spdlog::logger>& instance();
};

}