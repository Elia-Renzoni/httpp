
#include <iostream>
#include <gtest/gtest.h>

#include "parser.hpp"
#include "scanner.hpp"

using namespace server;

void assertMulti(const std::vector<SymbolPair>& got, const std::vector<std::pair<tokens, std::string>>& exp) {
    ASSERT_EQ(got.size(), exp.size());
    for (int i = 0; i < exp.size(); i++) {
        auto gotOut = got[i];
        auto expOut = exp[i];

        ASSERT_EQ(gotOut.token, expOut.first);
        ASSERT_EQ(gotOut.literal, expOut.second);
    }
}

TEST(TestParser, TestParseRequestLine) {
    std::array<std::string, 5> inputs = {
        "GET /index.html HTTP/1.1\r\n",
        "GET /api/users?id=42&active=true HTTP/1.1\r\n",
        "DELETE /api/users/42 HTTP/1.1\r\n",
        "POST /api/login HTTP/1.1\r\n",
        "CONNECT proxy.example.com:443 HTTP/1.1",
    };

    std::vector<std::vector<std::pair<tokens, std::string>>> outputs = {
        {{GET, "GET"}, {URL_ENDPOINT, "/index.html"}, {HTTP_1_1, "HTTP/1.1"}},
        {{GET, "GET"}, {URL_ENDPOINT, "/api/users"}, {URL_QUERY, "id"}, {URL_STRING, "42"}, {URL_QUERY, "active"}, {URL_STRING, "true"}, {HTTP_1_1, "HTTP/1.1"}},
        {{DELETE, "DELETE"}, {URL_ENDPOINT, "/api/users/42"}, {HTTP_1_1, "HTTP/1.1"}},
        {{POST, "POST"}, {URL_ENDPOINT, "/api/login"}, {HTTP_1_1, "HTTP/1.1"}},
        {{CONNECT, "CONNECT"}, {URL_HOST, "proxy.example.com:443"}, {HTTP_1_1, "HTTP/1.1"}},
    };

    for (int i = 0; i < inputs.size(); i++) {
        char *buf = inputs[i].data();

        server::TokensManager tm = server::TokensManager();
        server::Scanner s = server::Scanner(tm, buf, inputs[i].size());
        server::Parser p = server::Parser(s);

        try {
            p.parseRequestLine();
        } catch (ParserException& exp) {
            std::cout << exp.what();
            for (const auto& entry : p.parserStack->stack) {
                std::cout << "token: " << entry.token << " literal: " << entry.literal << "\n";
            }
            FAIL();
        }

        auto gotSymbolList = p.parserStack->stack;
        auto outputTestLine = outputs[i];
        assertMulti(gotSymbolList, outputTestLine);
    }
}

TEST(TestParser, TestCompleteHeader) {
    std::string inputBuffer = "Date: Thu, 02 Jul 2026 08:05:00 GMT\r\nContent-Type: text/html; charset=UTF-8\r\nContent-Length: 1845\r\nConnection: keep-alive\r\nCache-Control: no-cache, private";
    char *buf = inputBuffer.data();
    std::vector<std::pair<tokens, std::string>> expOut = {
        {DATE, "Date"},
        {STRING, "Thu, 02 Jul 2026 08:05:00 GMT"},
        {CONTENT_TYPE, "Content-Type"},
        {STRING, "text/html; charset=UTF-8"},
        {CONTENT_LENGTH, "Content-Length"},
        {STRING, "1845"},
        {CONNECTION, "Connection"},
        {STRING, "keep-alive"},
        {CACHE_CONTROL, "Cache-Control"},
        {STRING, "no-cache, private"},
    };

    server::TokensManager tm = server::TokensManager();
    server::Scanner s = server::Scanner(tm, buf, inputBuffer.size());
    server::Parser p = server::Parser(s);

    try {
        p.parseGenAndEntityHeader();
    } catch (ParserException& exp) {
        std::cout << exp.what();
        for (const auto& entry : p.parserStack->stack) {
            std::cout << "token: " << entry.token << " literal: " << entry.literal << "\n";
        }
        FAIL();
    }

    auto gotSymbolList = p.parserStack->stack;
    assertMulti(gotSymbolList, expOut);
}

// g++ -std=c++17 parser_test.cc parser.cc scanner.cc tokens.cpp -o parser_test -lgtest -lgtest_main -pthread

