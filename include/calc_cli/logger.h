#pragma once

#include <memory>
#include <spdlog/logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace calc_cli {

class Logger {
public:
    static std::shared_ptr<spdlog::logger>& instance() {

        static std::shared_ptr<spdlog::logger> logger = [] {
            auto logger_ptr = spdlog::stderr_color_mt("calc_cli_logger");
            logger_ptr->set_pattern("[%H:%M:%S] [%^%l%$] %v");
            logger_ptr->set_level(spdlog::level::info);
            return logger_ptr;
        }();

        return logger;
    }
    
    Logger() = delete;
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
};

}