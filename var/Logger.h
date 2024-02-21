#pragma once

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE  // needed for macro usage, before spdlog.h!
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/fmt/ostr.h"  // https://stackoverflow.com/questions/62320177/cannot-format-argument-from-within-spdlog

#define MASTER_LOG_LEVEL spdlog::level::trace
#define CONSOLE_LOG_LEVEL spdlog::level::trace
#define FILE_LOG_LEVEL spdlog::level::trace

class Logger
{
public:
    static void initialize(spdlog::level::level_enum masterLogLevel = MASTER_LOG_LEVEL);
};
