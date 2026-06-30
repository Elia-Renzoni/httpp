
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

std::pair<tokens, std::string> Scanner::scan(bool ignoreWhiteSpaces) {
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
        } else if (isWhiteSpace(ch) && ignoreWhiteSpaces) {
            continue;
        } else if (isEqual(ch, '=') || isEqual(ch, ';') || isEqual(ch, ',')) {
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
            break;
        }

        if (!isLetter(ch)) {
            if (!(isEqual(ch, '-') || isEqual(ch, '.') || isEqual(ch, '/')) && !isNumber(ch)) {
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
    tokens tok = UNKNOWN;

    for (;;) {
        char ch = readNext();

        if (isEqual(ch, BUF_EOF)) {
            tok = Scanner::fetchLatestState();
            break;
        }

        if (isWhiteSpace(ch)) {
            return {URL_WHITESPACE, buffer};
        }

        switch (Scanner::fetchLatestState()) {
            case URL_SCHEMA:
                if (isLetter(ch)) break;
                if (isNumber(ch)) return {UNKNOWN, buffer};
                if (isEqual(ch, ':')) {
                    Scanner::advanceState(URL_HOST);
                    goto EVAL_TOK;
                }
            case URL_HOST:
                if (isEqual(ch, '/')) {
                    char ch = readNext();
                    if (isEqual(ch, '/')) {
                        continue;
                    } else {
                        unread();
                        Scanner::advanceState(URL_ENDPOINT);
                        return {URL_HOST, buffer};
                    }
                }
 
                if (isLetter(ch) || isLegitSymbol(ch) || isNumber(ch)) break;

                if (isEqual(ch, '?')) {
                    Scanner::advanceState(URL_QUERY);
                    return {URL_HOST, buffer};
                }
            case URL_ENDPOINT:
                if (isLetter(ch) || isNumber(ch) || isEqual(ch, '/')) break;
                if (isEqual(ch, '?')) {
                    Scanner::advanceState(URL_QUERY);
                    return {URL_ENDPOINT, buffer};
                }

                if (isEqual(ch, '#')) {
                    Scanner::advanceState(URL_FRAGMENT);
                    return {URL_ENDPOINT, buffer};
                }
            case URL_QUERY:
                if (isLetter(ch) || isNumber(ch) || isLegitSymbol(ch)) break;
                if (isEqual(ch, '=')) {
                    Scanner::advanceState(URL_STRING);
                    return {URL_QUERY, buffer};
                }

            case URL_STRING:
                if (isLetter(ch) || isNumber(ch) || isLegitSymbol(ch)) break;
                if (isEqual(ch, '+')) {
                    // reconstruct white space
                    ch = ' ';
                    break;
                }

                if (isEqual(ch, '&')) {
                    Scanner::advanceState(URL_QUERY);
                    return {URL_STRING, buffer};
                }

                if (isEqual(ch, '#')) {
                    Scanner::advanceState(URL_FRAGMENT);
                    return {URL_STRING, buffer};
                }
            case URL_FRAGMENT:
                if (isLetter(ch) || isNumber(ch) || isLegitSymbol(ch)) break;
                else {
                    return {UNKNOWN, buffer};
                }

            default:
                break;
        }

        buffer.push_back(ch);
    }

EVAL_TOK:
    auto result = tokManager.getTokenFromLiteral(buffer);
    if (result.has_value())
        return {result.value(), buffer};

    return {tok, buffer};
}

void Scanner::resetStateMachineWidth(tokens initState) {
    stateMachine.clear();
    stateMachine.push_back(initState);
};

}
