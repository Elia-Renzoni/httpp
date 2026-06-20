
#include "scanner.hpp"

namespace server {

std::pair<tokens, std::string> Scanner::scanDigit() {
    std::string buffer;

    for (;;) {
        char ch = readNext();

        if (ch == BUF_EOF)
            break;

        if (isCR(ch)) {
            continue;
        }

        if (isLF(ch)) 
            return {INTEGER, buffer};

        // skip white space
        if (isWhiteSpace(ch)) continue;

        if (!isNumber(ch))
            break;

        buffer.push_back(ch);
    }

    if (buffer.empty()) {
        return {UNKNOWN, buffer};
    }

    return {INTEGER, buffer};
}

std::pair<tokens, std::string> Scanner::scanString() {
    std::string buffer;

    for (;;) {
        char ch = readNext();

        if (ch == BUF_EOF) 
            break;

        if (!isLetter(ch))
            break;

        buffer.push_back(ch);
    }

    if (buffer.empty())
        return {UNKNOWN, buffer};
    return {STRING, buffer};
};

std::pair<tokens, std::string> Scanner::scan() {
    std::string buffer;

    for (;;) {
        char ch = readNext();

        if (ch == BUF_EOF) 
            break;

        if (isCR(ch)) continue;

        if (isLF(ch)) break;;

        if (isLetter(ch)) {
            buffer.push_back(ch);
            continue;
        }

        if (isNumber(ch)) {
            buffer.push_back(ch);
            continue;
        }
        
        // ignore white space
        if (isWhiteSpace(ch)) {
            continue;
        }

        if (ch == '=') break;
        if (ch == ';') break;
        if (ch == ',') break;

        switch (ch) {
            case '-':
            case '.':
            case '_':
            case '~':
            case '/':
            case '?':
            case '&':
            case '%':
            case '+':
            case '@':
            case '#':
            case ',':
            case '$':
            case '<':
            case '>':
            case ':':
            case '`':
                buffer.push_back(ch);
                break;
            case '[':
                return {LPAR, "["};
            case ']':
                return {RPAR, buffer};
            default:
                return {UNKNOWN, buffer};
        }
    }


    auto token = tokManager.getTokenFromLiteral(buffer);
    if (token.has_value()) {
        return {token.value(), buffer};
    }
    return {STRING, buffer};
}

void Scanner::unscan(ssize_t positions) {
   currOffset -= positions;
};

std::pair<tokens, std::string> Scanner::scanKey() {
    std::string buffer;

    for (;;) {
        char ch = readNext();

        if (ch == BUF_EOF) break;

        if (isWhiteSpace(ch)) {
            return {UNKNOWN, buffer};
        }

        if (!isLetter(ch)) {
            if (!(ch == '-' || ch == '.' || ch == '/') && !isNumber(ch)) {
                if (ch != ':') {
                    return {UNKNOWN, buffer};
                } else {
                    break;
                }
            }
        }

        buffer.push_back(ch);
    }

    auto tok = tokManager.getTokenFromLiteral(buffer);
    if (tok.has_value()) {
        return {tok.value(), buffer};
    }
    return {UNKNOWN, buffer};
}

}
