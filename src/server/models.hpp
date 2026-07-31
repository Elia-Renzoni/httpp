
#include <string>
#include <unordered_map>
#include <vector>

namespace {

struct Request {
    std::string methodType;
    std::string protocolType;
    std::string endpoint;
    std::unordered_map<std::string, std::string> queryParameters;
    std::unordered_map<std::string, std::vector<std::string>> headers;
    int contentLength;
    std::string host;
    int remoteAddress;
    std::string body;
};

struct Response {
    std::string status;
    int statusCode;
    std::string protocolType;
    std::unordered_map<std::string, std::string> headers;
    int contentLength;
};

}
