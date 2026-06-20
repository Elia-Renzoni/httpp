
#include <string_view>
#include "tokens.hpp"

#pragma once

namespace server {

class Scanner {
    public:
        Scanner(TokensManager& tokManager, char* buf, ssize_t bufSize): tokManager(tokManager), buffer(buf, bufSize), currOffset(0){};
        ~Scanner() = default;

        std::pair<tokens, std::string> scanDigit();
        std::pair<tokens, std::string> scan();
        std::pair<tokens, std::string> scanString();
        std::pair<tokens, std::string> scanKey();
        void unscan(ssize_t positions);
    private:
        char readNext() {
            if (currOffset >= buffer.size()) return BUF_EOF;
            return buffer[currOffset++];
        };

        void unread() {
            currOffset -= 1;
        }

        bool isLetter(char ch) {
            return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
        }; 

        bool isNumber(char ch) {
            return ch >= '0' && ch <= '9';
        };

        bool isWhiteSpace(char ch) {
            return (ch == ' ') || (ch == '\t');
        };

        bool isCR(char ch) {
            return ch == '\r';
        }
        
        bool isLF(char ch) {
            return ch == '\n';
        }

        std::string_view buffer;
        size_t currOffset;
        const char BUF_EOF = '\0';
        TokensManager tokManager;
};

}
