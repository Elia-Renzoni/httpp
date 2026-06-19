
#include <gtest/gtest.h>

#include "scanner.hpp"

using namespace server;

TEST(ScannerTest, TestScanSimpleDigits) {
    std::string buffer = "1234";
    char* buf = buffer.data();

    server::TokensManager tm = server::TokensManager();
    server::Scanner s = server::Scanner(tm, buf, buffer.size());

    std::pair<tokens, std::string> scanResult = s.scanDigit();
    ASSERT_EQ(scanResult.first, INTEGER);
    ASSERT_EQ(scanResult.second, "1234");
}

TEST(ScannerTest, TestScanMixedBuffer) {
    std::string buffer = "1234a";
    char* buf = buffer.data();

    server::TokensManager tm = server::TokensManager();
    server::Scanner s = server::Scanner(tm, buf, buffer.size());

    std::pair<tokens, std::string> scanResult = s.scanDigit();
    ASSERT_EQ(scanResult.first, INTEGER);
    ASSERT_EQ(scanResult.second, "1234");
}

TEST(ScannerTest, TestScanMixedBuffer2) {
    std::string buffer = "12a34";
    char* buf = buffer.data();

    server::TokensManager tm = server::TokensManager();
    server::Scanner s = server::Scanner(tm, buf, buffer.size());

    std::pair<tokens, std::string> scanResult = s.scanDigit();
    ASSERT_EQ(scanResult.first, INTEGER);
    ASSERT_EQ(scanResult.second, "12");
}

TEST(ScannerTest, TestScanUnknownDigits) {
    std::string buffer = "abcdefg-";
    char* buf = buffer.data();

    server::TokensManager tm = server::TokensManager();
    server::Scanner s = server::Scanner(tm, buf, buffer.size());

    std::pair<tokens, std::string> scanResult = s.scanDigit();
    ASSERT_EQ(scanResult.first, UNKNOWN);
    ASSERT_EQ(scanResult.second, "");
}
