
#include <gtest/gtest.h>


TEST(TestParser, TestParseRequestLine) {
    std::array<std::string, 5> inputs = {
        "GET /index.html HTTP/1.1\r\n",
        "GET /api/users?id=42&active=true HTTP/1.1\r\n",
        "DELETE /api/users/42 HTTP/1.1\r\n",
        "POST /api/login HTTP/1.1\r\n",
        "CONNECT proxy.example.com:443 HTTP/1.1",
    };

    for (const auto& input : inputs) {
        const char *buf = input.c_str();
    }
}
