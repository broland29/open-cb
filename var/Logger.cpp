#include "Logger.h"

void initLogger(spdlog::level::level_enum masterLogLevel)
{
    // create sinks: multiple loggers may use this as output channel (since multithreaded)
    auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("log.txt");

    // configure log levels sinks accept
    consoleSink->set_level(CONSOLE_LOG_LEVEL);
    fileSink->set_level(FILE_LOG_LEVEL);
    fileSink->set_pattern("[%c] [%^%L%$] [%s:%#] [%!()] %v");

    // create a logger with name "myLogger"
    std::shared_ptr<spdlog::logger> myLogger = std::make_shared<spdlog::logger>("myLogger");

    // add sinks to myLogger
    myLogger->sinks().push_back(consoleSink);
    myLogger->sinks().push_back(fileSink);

    // set log level myLogger sends to sinks
    myLogger->set_level(masterLogLevel);

    // https://github.com/gabime/spdlog/wiki/3.-Custom-formatting#pattern-flags
    // for line and such, macros shall be used
    myLogger->set_pattern("[%^%L%$] [%s:%#] [%!()] %v");  // does not seem to work

    // could register and use it as such
    // spdlog::register_logger(myLogger);
    // spdlog::get("myLogger")->warn("Warning");

    // but since only one logger, set as default
    spdlog::set_default_logger(myLogger);

    SPDLOG_INFO("Logger initialized");
}