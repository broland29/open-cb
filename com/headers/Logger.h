#pragma once

// #define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_ERR  // needed for macro usage, before spdlog.h! - now defined in CMakeLists.txt
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>


class Logger
{
public:
    static void initialize();
};