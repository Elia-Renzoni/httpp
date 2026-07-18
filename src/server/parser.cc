
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

void Parser::parseGenAndEntityHeader() {
    std::pair<tokens, std::string> lexResult;

    SymbolPair sp;
    for (;;) {
        lexResult = lex.scanKey();
        if (lexResult.first == UNKNOWN) {
            lex.unscan(lexResult.second.size());
            throw ParserException("invalid header");
        }

        sp = SymbolPair{
            .token = lexResult.first,
            .literal = lexResult.second,
        };
        parserStack->push(sp);

        do {
            lexResult = lex.scan();
            if (lexResult.first == UNKNOWN) {
                throw ParserException("invalid header value");
            }
            sp = {lexResult.first, lexResult.second};
            parserStack->push(sp);
        } while (lex.lineBreakFound() && lexResult.first != CRLF);
        
    }
}

}
