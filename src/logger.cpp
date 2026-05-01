#include "../include/calc_cli/logger.h"

#include <memory>

namespace calc_cli {

std::shared_ptr<spdlog::logger>& Logger::instance() {

    static std::shared_ptr<spdlog::logger> logger = [] {
        auto logger_ptr = spdlog::stderr_color_mt("calc_cli_logger");
        logger_ptr->set_pattern("[%H:%M:%S] [%^%l%$] %v");
        logger_ptr->set_level(spdlog::level::info);
        return logger_ptr;
    }();

    return logger;
}

}