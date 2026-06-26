
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
    int httpSlashes = 0;
    tokens state;
    tokens tok = UNKNOWN;

    // http://www.google.com/search
    for (;;) {
        char ch = readNext();

        if (isEqual(ch, BUF_EOF) || isWhiteSpace(ch)) break;

        state = Scanner::fetchLatestState();
        switch (state) {
            case URL_SCHEMA:
                if (isLetter(ch)) break;
                if (isNumber(ch)) return {UNKNOWN, buffer};
                if (isEqual(ch, ':')) {
                    Scanner::advanceState(URL_HOST);
                    goto EVAL_TOK;
                }
            case URL_HOST:
                if (isEqual(ch, '/')) {
                    httpSlashes += 1;
                    continue;
                }
 
                if (isLetter(ch) || isLegitSymbol(ch) || isNumber(ch) || isEqual(ch, ':')) break;

                if (httpSlashes > 2) {
                    Scanner::advanceState(URL_ENDPOINT);
                    return {URL_HOST, buffer};
                }

                if (isEqual(ch, '?')) {
                    Scanner::advanceState(URL_QUERY);
                    return {URL_HOST, buffer};
                }
            case URL_ENDPOINT:
                if (isLetter(ch) || isNumber(ch)) break;
                if (isEqual(ch, '?')) {
                    Scanner::advanceState(URL_QUERY);
                    return {URL_ENDPOINT, buffer};
                }
            case URL_QUERY:
                break;
            case URL_FRAGMENT:
                break;
            default:
                break;
        }

        buffer.push_back(ch);
    }

EVAL_TOK:
    auto result = tokManager.getTokenFromLiteral(buffer);
    if (result.has_value())
        return {result.value(), buffer};

    return {UNKNOWN, buffer};
}

}
