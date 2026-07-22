#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>

enum class LogLevel { INFO, WARNING, ERROR_LOG };

class Logger {
public:
    static void log(LogLevel level, const std::string& message, const char* file, int line);
};

#define LOG_INFO(msg) Logger::log(LogLevel::INFO, msg, __FILE__, __LINE__)
#define LOG_WARN(msg) Logger::log(LogLevel::WARNING, msg, __FILE__, __LINE__)
#define LOG_ERROR(msg) Logger::log(LogLevel::ERROR_LOG, msg, __FILE__, __LINE__)

#endif // LOGGER_HPP
