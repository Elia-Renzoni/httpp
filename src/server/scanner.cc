
#include "scanner.hpp"

namespace server {

std::pair<tokens, std::string> Scanner::scanDigit() {
    std::string buffer;

    for (;;) {
        char ch = readNext();
        if (isNumber(ch)) {
            buffer.push_back(ch);
            continue;
        }

        unread();
        break;
    }

    if (buffer.empty())
        return {UNKNOWN, buffer};

    return {INTEGER, buffer};
}

std::pair<tokens, std::string> Scanner::scanString() {
    std::string buffer;

    for (;;) {
        char ch = readNext();
        if (isLetter(ch)) {
            buffer.push_back(ch);
        }

        unread();
        break;
    }

    if (buffer.empty())
        return {UNKNOWN, buffer};
    return {STRING, buffer};
};

std::pair<tokens, std::string> Scanner::scan() {
    std::string buffer;

    for (;;) {
        char ch = readNext();
        if (isLetter(ch)) {
            buffer.push_back(ch);
            continue;
        }

        if (isNumber(ch)) {
            buffer.push_back(ch);
            continue;
        }

        if (isWhiteSpace(ch)) {
            continue;
        }

        switch (ch) {
            case '-':
            case '.':
            case '_':
            case '~':
            case '/':
            case '?':
            case '&':
            case '=':
            case '%':
            case ':':
            case '+':
            case '@':
            case '#':
            case ';':
            case ',':
            case '$':
            case '<':
            case '>':
            case '`':
                buffer.push_back(ch);
            default:
                return {UNKNOWN, buffer};
        }
    }


    auto token = tokManager.getTokenFromLiteral(buffer);
    if (token.has_value())
        return {token.value(), buffer};
    return {UNKNOWN, buffer};
}

std::pair<tokens, std::string> Scanner::scanDateTime() {
    return {};
}

void Scanner::unscan(ssize_t positions) {
   currOffset -= positions;
};

}
