
#include "parser.hpp"

namespace server {

void Parser::parseRequestLine() {
    std::pair<tokens, std::string> result;

    result = lex.scanKey();

    if (!isValidMethodType(result.first)) {
        throw ParserException("invalid HTTP method type");
    }

    auto methodTypePair = SymbolPair{
        .token = result.first,
        .literal = result.second,
    };

    parserStack->push(methodTypePair);

    if (result.first != CONNECT)
        lex.resetStateMachineWidth(URL_ENDPOINT);
    else
        lex.resetStateMachineWidth(URL_HOST);

    SymbolPair urlPair;
    do {
        result = lex.scanURL();
        urlPair = SymbolPair{
            .token = result.first,
            .literal = result.second,
        };

        parserStack->push(urlPair);
    } while (isValidURL(result.first));
    // delete white space state
    parserStack->pop();

    if (!checkURLOrderGoodness(parserStack)) {
        throw ParserException("invalid URL order");
    }

    result = lex.scanKey();
    if (!isValidProtocol(result.first)) {
        throw ParserException("invalid HTTP protocol type");
    }

    auto protocolPair = SymbolPair{
        .token = result.first,
        .literal = result.second,
    };

    parserStack->push(protocolPair);
}

void Parser::parseGeneralHeader() {
}

void Parser::parseEntityHeader() {
}

}
