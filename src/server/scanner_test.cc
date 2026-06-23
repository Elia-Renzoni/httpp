
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
    std::string buffer = "ab%<>";
    char* buf = buffer.data();

    server::TokensManager tm = server::TokensManager();
    server::Scanner s = server::Scanner(tm, buf, buffer.size());

    std::pair<tokens, std::string> scanResult = s.scan();
    ASSERT_EQ(scanResult.first, STRING);
    ASSERT_EQ(scanResult.second, "ab%<>");
}

TEST(ScannerTest, TestScanRandomString) {
    std::string buffer = "U12-";
    char* buf = buffer.data();

    server::TokensManager tm = server::TokensManager();
    server::Scanner s = server::Scanner(tm, buf, buffer.size());

    std::pair<tokens, std::string> scanResult = s.scan();
    ASSERT_EQ(scanResult.first, STRING);
    ASSERT_EQ(scanResult.second, "U12-");
}

TEST(ScannerTest, TestScanHeaderKey) {
    std::string buffer = "User-Agent:";
    char* buf = buffer.data();

    server::TokensManager tm = server::TokensManager();
    server::Scanner s = server::Scanner(tm, buf, buffer.size());

    std::pair<tokens, std::string> scanResult = s.scanKey();
    ASSERT_EQ(scanResult.first, USER_AGENT);
    ASSERT_EQ(scanResult.second, "User-Agent");
}

TEST(ScannerTest, TestScanHeaderKeyWithErrors) {
    std::string buffer = "User-Agent : ";
    char* buf = buffer.data();

    server::TokensManager tm = server::TokensManager();
    server::Scanner s = server::Scanner(tm, buf, buffer.size());

    std::pair<tokens, std::string> scanResult = s.scanKey();
    ASSERT_EQ(scanResult.first, UNKNOWN);
    ASSERT_EQ(scanResult.second, "User-Agent");
}

TEST(ScannerTest, TestScanHTTPVersion2) {
    std::string buffer = "HTTP/2";
    char* buf = buffer.data();

    server::TokensManager tm = server::TokensManager();
    server::Scanner s = server::Scanner(tm, buf, buffer.size());

    std::pair<tokens, std::string> scanResult = s.scanKey();
    ASSERT_EQ(scanResult.first, HTTP_2);
    ASSERT_EQ(scanResult.second, "HTTP/2");
}

TEST(ScannerTest, TestScanHTTPVersion11) {
    std::string buffer = "HTTP/1.1";
    char* buf = buffer.data();

    server::TokensManager tm = server::TokensManager();
    server::Scanner s = server::Scanner(tm, buf, buffer.size());

    std::pair<tokens, std::string> scanResult = s.scanKey();
    ASSERT_EQ(scanResult.first, HTTP_1_1);
    ASSERT_EQ(scanResult.second, "HTTP/1.1");
}

TEST(ScannerTest, TestScanHTTPVersion3) {
    std::string buffer = "HTTP/3";
    char* buf = buffer.data();

    server::TokensManager tm = server::TokensManager();
    server::Scanner s = server::Scanner(tm, buf, buffer.size());

    std::pair<tokens, std::string> scanResult = s.scanKey();
    ASSERT_EQ(scanResult.first, HTTP_3);
    ASSERT_EQ(scanResult.second, "HTTP/3");
}

TEST(ScannerTest, TestScanComplexExpressions) {
    std::string buffer = "text/html; charset=UTF-8";
    char *buf = buffer.data();

    server::TokensManager tm = server::TokensManager();
    server::Scanner s = server::Scanner(tm, buf, buffer.size());

    std::pair<tokens, std::string> scanResult = s.scan();
    ASSERT_EQ(scanResult.first, STRING);
    ASSERT_EQ(scanResult.second, "text/html");

    scanResult = s.scan();
    ASSERT_EQ(scanResult.first, STRING);
    ASSERT_EQ(scanResult.second, "charset");

    scanResult = s.scan();
    ASSERT_EQ(scanResult.first, STRING);
    ASSERT_EQ(scanResult.second, "UTF-8");
}

TEST(ScannerTest, TestScanHeaderKVPair) {
    std::string buffer = "Content-Type: multipart/form-data; boundary=something";
    char *buf = buffer.data();

    server::TokensManager tm = server::TokensManager();
    server::Scanner s = server::Scanner(tm, buf, buffer.size());

    std::pair<tokens, std::string> scanResult = s.scanKey();
    ASSERT_EQ(scanResult.first, CONTENT_TYPE);
    ASSERT_EQ(scanResult.second, "Content-Type");

    scanResult = s.scan();
    ASSERT_EQ(scanResult.first, STRING);
    ASSERT_EQ(scanResult.second, "multipart/form-data");

    scanResult = s.scan();
    ASSERT_EQ(scanResult.first, STRING);
    ASSERT_EQ(scanResult.second, "boundary");

    scanResult = s.scan();
    ASSERT_EQ(scanResult.first, STRING);
    ASSERT_EQ(scanResult.second, "something");
}

TEST(ScannerTest, TestScanHeaderKVPair2) {
    std::string buffer = "Content-Length: 348";
    char *buf = buffer.data();

    server::TokensManager tm = server::TokensManager();
    server::Scanner s = server::Scanner(tm, buf, buffer.size());

    std::pair<tokens, std::string> scanResult = s.scanKey();
    ASSERT_EQ(scanResult.first, CONTENT_LENGTH);
    ASSERT_EQ(scanResult.second, "Content-Length");

    scanResult = s.scanDigit();
    ASSERT_EQ(scanResult.first, INTEGER);
    ASSERT_EQ(scanResult.second, "348");
}

TEST(ScannerTest, TestScanCRLF) {
    std::string buffer = "Content-Length: 348\r\n";
    char *buf = buffer.data();

    server::TokensManager tm = server::TokensManager();
    server::Scanner s = server::Scanner(tm, buf, buffer.size());

    std::pair<tokens, std::string> scanResult = s.scanKey();
    ASSERT_EQ(scanResult.first, CONTENT_LENGTH);
    ASSERT_EQ(scanResult.second, "Content-Length");

    scanResult = s.scanDigit();
    ASSERT_EQ(scanResult.first, INTEGER);
    ASSERT_EQ(scanResult.second, "348");
}

TEST(ScannerTest, TestScanCRLF2) {
    std::string buffer = "Content-Type: application/json; charset=utf-8\r\n";
    char *buf = buffer.data();

    server::TokensManager tm = server::TokensManager();
    server::Scanner s = server::Scanner(tm, buf, buffer.size());

    std::pair<tokens, std::string> scanResult = s.scanKey();
    ASSERT_EQ(scanResult.first, CONTENT_TYPE);
    ASSERT_EQ(scanResult.second, "Content-Type");

    scanResult = s.scan();
    ASSERT_EQ(scanResult.first, STRING);
    ASSERT_EQ(scanResult.second, "application/json");

    scanResult = s.scan();
    ASSERT_EQ(scanResult.first, STRING);
    ASSERT_EQ(scanResult.second, "charset");

    scanResult = s.scan();
    ASSERT_EQ(scanResult.first, STRING);
    ASSERT_EQ(scanResult.second, "utf-8");
}

TEST(ScannerTest, TestScanCompleteHeader) {
    std::string buffer = "User-Agent: Mozilla/5.0\r\nAccept: text/html\r\nConnection: keep-alive\r\n";
    char *buf = buffer.data();

    server::TokensManager tm = server::TokensManager();
    server::Scanner s = server::Scanner(tm, buf, buffer.size());

    std::vector<std::pair<tokens, std::string>> exp = {
        {USER_AGENT, "User-Agent"},
        {STRING, "Mozilla/5.0"},
        {ACCEPT, "Accept"},
        {STRING, "text/html"},
        {CONNECTION, "Connection"},
        {STRING, "keep-alive"},
    };

    std::pair<tokens, std::string> result;
    bool swapTime = false;
    for (auto &expPair: exp) {
        if (!swapTime) {
            result = s.scanKey();
            swapTime = true;
        } else {
            result = s.scan();
            swapTime = false;
        }

        ASSERT_EQ(expPair.first, result.first);
        ASSERT_EQ(expPair.second, result.second);
    }
}
