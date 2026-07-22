#include <gtest/gtest.h>
#include <iostream>
#include <sstream>
#include "logger.hpp"

std::string captureOutput(std::function<void()> func) {
    std::stringstream buffer;
    std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());
    
    func(); 
    
    std::cout.rdbuf(oldCout); 
    return buffer.str();
}

TEST(LoggerTestCase, VerifyFormatAndContent) {
    std::string output = captureOutput([]() {
        LOG_INFO("TestMessage");
    });

    EXPECT_NE(output.find("[INFO]"), std::string::npos);
    EXPECT_NE(output.find("TestMessage"), std::string::npos);
}

// g++ logger.cc logger_test.cc -lgtest -lgtest_main -o test_logger

