
#include <vector>
#include <string_view>
#include "tokens.hpp"

#pragma once

namespace parsing {

class Scanner {
    public:
        Scanner(TokensManager& tokManager, char* buf, ssize_t bufSize): tokManager(tokManager), buffer(buf, bufSize), currOffset(0){};
        ~Scanner() = default;

        std::pair<tokens, std::string> scanDigit();
        std::pair<tokens, std::string> scan();
        std::pair<tokens, std::string> scanString();
        std::pair<tokens, std::string> scanKey();
        std::pair<tokens, std::string> scanURL();
        bool isLineEnd();
        bool isEOF();
        void unscan(ssize_t positions);
        void resetStateMachineWidth(tokens initState);
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

        bool isEqual(char in, char sentinel) {
            return in == sentinel;
        }

        bool isNotEqual(char in, char sentinel) {
            return !(in == sentinel);
        }

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

        bool isLegitSymbol(char inputSymbol) {
            switch (inputSymbol) {
                case '-':
                case '.':
                case '_':
                case '~':
                case '/':
                case '%':
                case '@':
                case '$':
                case '<':
                case '>':
                case ':':
                case '`':
                case '[':
                case ']':
                    return true;
                default:
                    return false;
            }
        }

        tokens fetchLatestState() {
            return stateMachine.back();
        }

        void advanceState(tokens newState) {
            stateMachine.push_back(newState);
        }

        std::string_view buffer;
        size_t currOffset;
        const char BUF_EOF = '\0';
        TokensManager tokManager;
        std::vector<tokens> stateMachine = {URL_SCHEMA};
        bool lineBreak;
};

}
