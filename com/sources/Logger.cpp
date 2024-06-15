#include "../headers/Logger.h"


void Logger::initialize()
{
    auto logger = spdlog::stdout_color_mt("opencb_logger");
    
    logger->set_pattern("[%^%L%$] [%s:%#] [%!()] %v");
    spdlog::set_default_logger(logger);
    spdlog::set_level(spdlog::level::trace);

    SPDLOG_TRACE("Logger initialized.");
}