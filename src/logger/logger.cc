#include "logger.hpp"
#include <iostream>
#include <chrono>
#include <iomanip>

void Logger::log(LogLevel level, const std::string& message, const char* file, int line) {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    
    std::string levelStr;
    switch (level) {
        case LogLevel::INFO:    levelStr = "[INFO]   "; break;
        case LogLevel::WARNING: levelStr = "[WARNING]"; break;
        case LogLevel::ERROR_LOG:   levelStr = "[ERROR]  "; break;
    }

    std::cout << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S")
              << " " << levelStr << " "
              << "[" << file << ":" << line << "] "
              << message << std::endl;
}
