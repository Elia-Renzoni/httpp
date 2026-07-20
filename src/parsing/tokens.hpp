
#include <string>
#include <optional>
#include <unordered_map>

namespace parsing {

enum tokens {
    // HTTP methods
    OPTIONS = 0,
    GET = 1,
    HEAD = 2,
    POST = 3,
    PUT = 4,
    DELETE = 5,
    TRACE = 6,
    CONNECT = 7,
    PATCH = 8,

    // HTTP versions
    HTTP_1 = 9,
    HTTP_1_1 = 10,
    HTTP_2 = 11,
    HTTP_3 = 12,

    // Headers
    CACHE_CONTROL = 13,
    CONNECTION = 14,
    DATE = 15,
    PRAGMA = 16,
    TRAILER = 17,
    TRANSFER_ENCODING = 18,
    UPGRADE = 19,
    VIA = 20,
    WARNING = 21,

    ACCEPT = 22,
    ACCEPT_CHARSET = 23,
    ACCEPT_ENCODING = 24,
    ACCEPT_LANGUAGE = 25,
    AUTHORIZATION = 26,
    EXCEPT = 27,
    FROM = 28,
    HOST = 29,
    IF_MATCH = 30,
    IF_MODIFIED_SINCE = 31,
    IF_NONE_MATCH = 32,
    IF_RANGE = 33,
    IF_UNMODIFIED_SINCE = 34,
    MAX_FORWARDS = 35,
    PROXY_AUTHORIZATION = 36,
    RANGE = 37,
    REFERER = 38,
    TE = 39,
    USER_AGENT = 40,

    ALLOW = 41,
    CONTENT_ENCODING = 42,
    CONTENT_LANGUAGE = 43,
    CONTENT_LENGTH = 44,
    CONTENT_LOCATION = 45,
    CONTENT_MD5 = 46,
    CONTENT_RANGE = 47,
    CONTENT_TYPE = 48,
    EXPIRES = 49,
    LAST_MODIFIED = 50,
    EXSTENSION_HEADER = 51,

    // control chars / special
    SP = 52,
    CR = 53,
    LF = 54,
    CRLF = 55,

    INTEGER = 56,
    STRING = 57,
    DATETIME = 58,
    UNKNOWN = 59,

    LPAR = 60,
    RPAR = 61, 

    // for state machine lexing
    URL_SCHEMA = 62,
    URL_SCHEMAS = 63,
    URL_HOST = 64,
    URL_ENDPOINT = 65,
    URL_QUERY = 66,
    URL_STRING = 67,
    URL_FRAGMENT = 68,
    URL_WHITESPACE = 69,

};

class TokensManager {
    public:
        TokensManager();
        ~TokensManager() = default;

        std::optional<tokens> getTokenFromLiteral(std::string& literal);
        std::optional<std::string> getLiteralFromToken(tokens& token);
    private:
        void createAndFillTokenMap() {
            tokenMap_ = {
                {OPTIONS, "OPTIONS"},
                {GET, "GET"},
                {HEAD, "HEAD"},
                {POST, "POST"},
                {PUT, "PUT"},
                {DELETE, "DELETE"},
                {TRACE, "TRACE"},
                {CONNECT, "CONNECT"},
                {PATCH, "PATCH"},

                {HTTP_1, "HTTP/1"},
                {HTTP_1_1, "HTTP/1.1"},
                {HTTP_2, "HTTP/2"},
                {HTTP_3, "HTTP/3"},

                {CACHE_CONTROL, "Cache-Control"},
                {CONNECTION, "Connection"},
                {DATE, "Date"},
                {PRAGMA, "Pragma"},
                {TRAILER, "Trailer"},
                {TRANSFER_ENCODING, "Transfer-Encoding"},
                {UPGRADE, "Upgrade"},
                {VIA, "Via"},
                {WARNING, "Warning"},

                {ACCEPT, "Accept"},
                {ACCEPT_CHARSET, "Accept-Charset"},
                {ACCEPT_ENCODING, "Accept-Encoding"},
                {ACCEPT_LANGUAGE, "Accept-Language"},
                {AUTHORIZATION, "Authorization"},
                {EXCEPT, "Expect"},
                {FROM, "From"},
                {HOST, "Host"},
                {IF_MATCH, "If-Match"},
                {IF_MODIFIED_SINCE, "If-Modified-Since"},
                {IF_NONE_MATCH, "If-None-Match"},
                {IF_RANGE, "If-Range"},
                {IF_UNMODIFIED_SINCE, "If-Unmodified-Since"},
                {MAX_FORWARDS, "Max-Forwards"},
                {PROXY_AUTHORIZATION, "Proxy-Authorization"},
                {RANGE, "Range"},
                {REFERER, "Referer"},
                {TE, "TE"},
                {USER_AGENT, "User-Agent"},

                {ALLOW, "Allow"},
                {CONTENT_ENCODING, "Content-Encoding"},
                {CONTENT_LANGUAGE, "Content-Language"},
                {CONTENT_LENGTH, "Content-Length"},
                {CONTENT_LOCATION, "Content-Location"},
                {CONTENT_MD5, "Content-MD5"},
                {CONTENT_RANGE, "Content-Range"},
                {CONTENT_TYPE, "Content-Type"},
                {EXPIRES, "Expires"},
                {LAST_MODIFIED, "Last-Modified"},
                {EXSTENSION_HEADER, "Extension-Header"},

                {SP, " "},
                {CR, "\r"},
                {LF, "\n"},
                {CRLF, "\r\n"},
                {LPAR, "["},
                {RPAR, "]"},
                {URL_SCHEMA, "http"},
                {URL_SCHEMAS, "https"}
            };
        };

        std::unordered_map<tokens, std::string> tokenMap_;
};

}
