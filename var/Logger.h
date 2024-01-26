#pragma once

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE  // needed for macro usage, before spdlog.h!
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

#define MASTER_LOG_LEVEL spdlog::level::trace
#define CONSOLE_LOG_LEVEL spdlog::level::trace
#define FILE_LOG_LEVEL spdlog::level::trace

void initLogger(spdlog::level::level_enum masterLogLevel = MASTER_LOG_LEVEL);
