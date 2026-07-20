
#include "tokens.hpp"

namespace parsing  {

TokensManager::TokensManager() {
    createAndFillTokenMap();
};

std::optional<std::string> TokensManager::getLiteralFromToken(tokens& token) {
    if (!tokenMap_.count(token)) {
        return std::nullopt;
    }
    return tokenMap_[token];
};

std::optional<tokens> TokensManager::getTokenFromLiteral(std::string& literal) {
    for (const auto& [tokenID, tokenLiteral] : tokenMap_) {
        if (tokenLiteral == literal) {
            return tokenID;
        }
    }
    return std::nullopt;
};
 
};
