
#include <string>
#include <optional>
#include <unordered_map>

namespace server {

enum tokens {
    OPTIONS, GET, HEAD, POST, PUT, DELETE, TRACE, CONNECT, PATCH,

    HTTP_1, HTTP_1_1, HTTP_2, HTTP_3,

    // TODO-> missing URI-related tokens

    CACHE_CONTROL,
    CONNECTION,
    DATE,
    PRAGMA,
    TRAILER,
    TRANSFER_ENCODING,
    UPGRADE,
    VIA,
    WARNING,

    ACCEPT, ACCEPT_CHARSET, ACCEPT_ENCODING, ACCEPT_LANGUAGE, 
    AUTHORIZATION, EXCEPT, FROM, HOST, IF_MATCH, IF_MODIFIED_SINCE,
    IF_NONE_MATCH, IF_RANGE, IF_UNMODIFIED_SINCE, MAX_FORWARDS,
    PROXY_AUTHORIZATION, RANGE, REFERER, TE, USER_AGENT,


    ALLOW, CONTENT_ENCODING, CONTENT_LANGUAGE, CONTENT_LENGTH, 
    CONTENT_LOCATION, CONTENT_MD5, CONTENT_RANGE, CONTENT_TYPE,
    EXPIRES, LAST_MODIFIED, EXSTENSION_HEADER,

    SP,
    CR,
    LF,
    CRLF,


    INTEGER,
    STRING,
    DATETIME,
    UNKNOWN
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
                {CRLF, "\r\n"}
            };
        };

        std::unordered_map<int, std::string> tokenMap_;
};

}
