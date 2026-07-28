
#include <string>
#include <unordered_map>

namespace {

struct Request {
    std::string methodType;
    std::string protocolType;
    std::unordered_map<std::string, std::string> headers;
    int contentLength;
    std::string host;
    int remoteAddress;
};

struct Response {
    std::string status;
    int statusCode;
    std::string protocolType;
    std::unordered_map<std::string, std::string> headers;
    int contentLength;
};


}
