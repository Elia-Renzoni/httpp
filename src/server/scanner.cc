
#include <iostream>
#include "scanner.hpp"

namespace server {

std::pair<tokens, std::string> Scanner::scanDigit() {
    std::string buffer;
    bool crSymbol = false;

    for (;;) {
        char ch = readNext();

        if (isCR(ch)) {
            if (crSymbol) {
                buffer.clear();
                break;
            }
            crSymbol = true;
            continue;
        } else if (isLF(ch)) {
            if (crSymbol) {
                break;
            } else {
                buffer.clear();
                break;
            }
        } else if (isWhiteSpace(ch)) {
            continue;
        } else if (ch == BUF_EOF || !isNumber(ch)) {
            break;
        }

        buffer.push_back(ch);
    }

    if (buffer.empty()) {
        return {UNKNOWN, buffer};
    }

    return {INTEGER, buffer};
}

std::pair<tokens, std::string> Scanner::scanString() {
    std::string buffer;
    bool crSymbol = false;

    for (;;) {
        char ch = readNext();

        if (isCR(ch)) {
            if (crSymbol) {
                buffer.clear();
                break;
            }
            crSymbol = true;
        } else if (isLF(ch)) {
            if (crSymbol) {
                break;
            } else {
                buffer.clear();
                break;
            }
        } else if (isWhiteSpace(ch)) {
            continue;
        } else if (ch == BUF_EOF || !isLetter(ch)) {
            break;
        }

        buffer.push_back(ch);
    }

    if (buffer.empty())
        return {UNKNOWN, buffer};
    return {STRING, buffer};
};

std::pair<tokens, std::string> Scanner::scan() {
    std::string buffer;
    bool crSymbol = false;

    for (;;) {
        char ch = readNext();

        if (isCR(ch)) {
            if (crSymbol) {
                buffer.clear();
                break;
            }

            crSymbol = true;
            continue;
        } else if (isLF(ch)) {
            if (crSymbol) {
                break;
            } else {
                buffer.clear();
                break;
            }
        } else if (ch == BUF_EOF) {
            break;
        } else if (isLetter(ch) || isNumber(ch)) {
            buffer.push_back(ch);
            continue;
        } else if (isWhiteSpace(ch)) {
            continue;
        } else if (ch == '=' || ch == ';' || ch == ',') {
            break;
        }

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

std::pair<tokens, std::string> Scanner::scanURL() {
    std::string buffer;
    bool crSymbol = false;
    int slashes = 0;
    tokens matchedToken = UNKNOWN;

    for (;;) {
        char ch = readNext();
        std::cout << "reading: " << ch << "\n";

        if (isEqual(ch, BUF_EOF)) break;

        if (isEqual(ch, ':') && !isNumber(readNext())) {
            unread();
            break;
        }

        if (isEqual(ch, '/') && slashes < 2) {
            slashes += 1;
            continue;
        }

        if ((isEqual(ch, '?') || isEqual(ch, '/')) && slashes >= 2) {
            matchedToken = URL_HOST;
            break;
        } else if (isEqual(ch, '?') && slashes == 1) {
            matchedToken = URL_ENDPOINT;
            break;
        } else if (isEqual(ch, '=')) {
            matchedToken = URL_QUERY;
            break;
        } else if (isEqual(ch, '&')) {
            // FIXME: what about STRING vs INTEGER?
            matchedToken = STRING;
            break;
        }

        if (isLetter(ch) || isNumber(ch) || isLegitSymbol(ch)) {
            buffer.push_back(ch);
        }
    }

    auto token = tokManager.getTokenFromLiteral(buffer);
    if (token.has_value()) 
        return {token.value(), buffer};

    return {matchedToken, buffer};
}

}
