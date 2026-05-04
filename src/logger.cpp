#include "../include/calc_cli/logger.h"

#include <memory>

namespace calc_cli {
namespace {

std::shared_ptr<spdlog::logger>& raw_logger() {

    static std::shared_ptr<spdlog::logger> logger = [] {
        auto logger_ptr = spdlog::stderr_color_mt("calc_cli_logger");
        logger_ptr->set_pattern("[%H:%M:%S] [%^%l%$] %v");
        logger_ptr->set_level(spdlog::level::info);
        return logger_ptr;
    }();

    return logger;
}

}

Logger& Logger::instance() {
    static Logger logger;
    return logger;
}

void Logger::trace(std::string_view message) const {
    raw_logger()->trace("{}", message);
}

void Logger::debug(std::string_view message) const {
    raw_logger()->debug("{}", message);
}

void Logger::info(std::string_view message) const {
    raw_logger()->info("{}", message);
}

void Logger::warn(std::string_view message) const {
    raw_logger()->warn("{}", message);
}

void Logger::error(std::string_view message) const {
    raw_logger()->error("{}", message);
}

void Logger::critical(std::string_view message) const {
    raw_logger()->critical("{}", message);
}

}