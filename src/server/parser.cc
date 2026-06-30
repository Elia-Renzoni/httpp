
#include "parser.hpp"

namespace server {

void Parser::parseRequestLine() {
    std::pair<tokens, std::string> result;

    result = lex.scanKey();

    if (!Parser::isValidProtocol(result.first)) {
        throw ParserException("invalid HTTP protocol type");
    }

    auto protocolPair = SymbolPair{
        .token = result.first,
        .literal = result.second,
    };

    parserStack->push(protocolPair);

    SymbolPair urlPair;
    do {
        result = lex.scanURL();
        urlPair = SymbolPair{
            .token = result.first,
            .literal = result.second,
        };

        parserStack->push(urlPair);
    } while (isValidURL(result.first));

    if (!checkURLOrderGoodness(parserStack)) {
        parserStack->clearAll();
        throw ParserException("invalid URL token order");
    }

    result = lex.scan(!IGNORE_WHITE_SPACES);
    if (!isValidMethodType(result.first)) {
        throw ParserException("invalid HTTP method type");
    }

    auto methodTypePair = SymbolPair{
        .token = result.first,
        .literal = result.second,
    };

    parserStack->push(methodTypePair);
}

void Parser::parseGeneralHeader() {
}

void Parser::parseEntityHeader() {
}

}
