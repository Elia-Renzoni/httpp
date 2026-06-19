
#include <gtest/gtest.h>

#include "scanner.hpp"

using namespace server;

TEST(ScannerTest, TestScanSimpleDigits) {
    std::string buffer = "1234";
    char* buf = buffer.data();

    server::TokensManager tm = server::TokensManager();
    server::Scanner s = server::Scanner(tm, buf, buffer.size());

    std::pair<tokens, std::string> scanResult = s.scanDigit();

    ASSERT_EQ(scanResult.second, "1234");
    ASSERT_EQ(scanResult.first, INTEGER);
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

TEST(ScannerTest, TestScanStrings) {
    std::string buffer = "abcdefg";
    char* buf = buffer.data();

    server::TokensManager tm = server::TokensManager();
    server::Scanner s = server::Scanner(tm, buf, buffer.size());

    std::pair<tokens, std::string> scanResult = s.scanString();
    ASSERT_EQ(scanResult.first, STRING);
    ASSERT_EQ(scanResult.second, "abcdefg");
}

TEST(ScannerTest, TestScanStringsMixed) {
    std::string buffer = "ab%";
    char* buf = buffer.data();

    server::TokensManager tm = server::TokensManager();
    server::Scanner s = server::Scanner(tm, buf, buffer.size());

    std::pair<tokens, std::string> scanResult = s.scanString();
    ASSERT_EQ(scanResult.first, STRING);
    ASSERT_EQ(scanResult.second, "ab");
}

TEST(ScannerTest, TestGeneralScan) {
    std::string buffer = "ab%<>=b";
    char* buf = buffer.data();

    server::TokensManager tm = server::TokensManager();
    server::Scanner s = server::Scanner(tm, buf, buffer.size());

    std::pair<tokens, std::string> scanResult = s.scan();
    ASSERT_EQ(scanResult.first, STRING);
    ASSERT_EQ(scanResult.second, "ab%<>=b");
}

TEST(ScannerTest, TestScanUserAgent) {
    std::string buffer = "U12-";
    char* buf = buffer.data();

    server::TokensManager tm = server::TokensManager();
    server::Scanner s = server::Scanner(tm, buf, buffer.size());

    std::pair<tokens, std::string> scanResult = s.scan();
    ASSERT_EQ(scanResult.first, USER_AGENT);
    ASSERT_EQ(scanResult.second, "U12-");
}
