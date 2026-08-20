#include "logger.hpp"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>

void Logger::log(LogLevel level, const std::string& message, const char* file, int line) {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    
    struct tm timeinfo;
    localtime_r(&time, &timeinfo); 

    std::string levelStr;
    switch (level) {
        case LogLevel::INFO:      levelStr = "[INFO]   "; break;
        case LogLevel::WARNING:   levelStr = "[WARNING]"; break;
        case LogLevel::ERROR_LOG: levelStr = "[ERROR]  "; break;
    }

    std::ostringstream ss;
    ss << std::put_time(&timeinfo, "%Y-%m-%d %H:%M:%S")
       << " " << levelStr << " "
       << "[" << file << ":" << line << "] "
       << message << "\n";

    std::cout << ss.str() << std::flush;
}
