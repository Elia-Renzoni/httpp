
#include <algorithm>
#include "tokens.hpp"

namespace server  {

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
    tokens t;
    std::for_each(tokenMap_.begin(), tokenMap_.end(), [&t, literal](const auto& entry) {
            const auto& [tokenID, tokenLiteral] = entry;

            if (tokenLiteral == literal) {
                t = tokenID;
            }
    });
    return t;
};
 
};
