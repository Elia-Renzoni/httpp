
#include <string>
#include <memory>
#include <vector>
#include <algorithm>
#include <stdexcept>

#include "scanner.hpp"

#pragma once

namespace server {

class ParserException : public std::runtime_error {
    public:
        explicit ParserException(const std::string& msg): std::runtime_error(msg) {}
};

struct SymbolPair {
    tokens token;
    std::string literal;

    bool operator<(SymbolPair& next) {
        return token < next.token;
    }
};

struct PStack {
    std::vector<SymbolPair> stack;
    int offset;
    int watermark;

    PStack(): offset(0), watermark(0) {}
    ~PStack() = default;

    void push(SymbolPair& p) {
        stack.push_back(p);
    }

    void pop() {
        stack.pop_back();
    }

    std::vector<SymbolPair>::iterator getStackIterator() {
        return stack.begin();
    }

    void clearAll() {
        stack.clear();
    }
};

class Parser {
    public:
        Parser(Scanner& lexer): 
           lex(lexer), parserStack(std::make_unique<PStack>()) {};
        ~Parser() = default;

        void parseRequestLine();
        void parseGenAndEntityHeader();

        std::unique_ptr<PStack> parserStack;
    private:
        bool isValidProtocol(const tokens& proto) {
            tokens toks[] = {HTTP_1, HTTP_1_1, HTTP_2, HTTP_3};
            for (const auto& tok : toks) {
                if (tok == proto) return true;
            }
            return false;
        }

        bool isValidURL(const tokens& urlToken) {
            tokens states[] = {URL_ENDPOINT, URL_HOST, URL_STRING, URL_SCHEMA, URL_SCHEMAS, URL_QUERY};

            for (const auto& state : states) {
                if (state == urlToken) return true;
            }
            return false;
        }

        bool checkURLOrderGoodness(const std::unique_ptr<PStack>& pStack) {
            auto stack = pStack->stack;
            auto watermark = is_sorted_until(stack.begin(), stack.end());
            if (watermark != stack.end()) {
                auto offset = std::distance(stack.begin(), watermark);
                auto faultyEntry = stack[offset];
                if (faultyEntry.token == URL_QUERY) {
                    auto adjEntry = stack[offset+1];
                    if (adjEntry.token == URL_STRING) {
                        return true;
                    }
                }
            } else {
                return true;
            }

            return false;
        }

        bool isValidHeader(const bool& caller, const tokens& headerTokType) {
            tokens entityHeadToks[] = {
                CONTENT_TYPE, CONTENT_LENGTH, CONTENT_ENCODING,
                CONTENT_LANGUAGE, CONTENT_LOCATION, CONTENT_RANGE,
                CONTENT_MD5, EXPIRES, LAST_MODIFIED
            };
            bool result;

            for (const auto& tok : entityHeadToks) {
                if (caller == CALLER_IS_ENTITYH && tok == headerTokType)
                    result = true;
             
                if (caller == CALLER_IS_GENERALH && tok == headerTokType)
                    result = false;
            }
            return result;
        }

        bool isValidMethodType(const tokens& methodType) {
            tokens methodTypes[] = {GET, HEAD, POST, PATCH, DELETE, OPTIONS, PUT, TRACE, CONNECT};

            for (const auto& method : methodTypes) {
                if (method == methodType) return true;
            }
            return false;
        }

        Scanner& lex;
        const bool IGNORE_WHITE_SPACES = false;
        const bool CALLER_IS_ENTITYH = true;
        const bool CALLER_IS_GENERALH = false;
};

}
