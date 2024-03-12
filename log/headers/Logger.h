#pragma once

// #define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_ERR  // needed for macro usage, before spdlog.h! - defined in CMakeLists.txt
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
// #include "spdlog/fmt/ostr.h" https://stackoverflow.com/questions/62320177/cannot-format-argument-from-within-spdlog


class Logger
{
public:
    static void initialize();
};