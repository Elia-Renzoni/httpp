#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <sstream>

#include "../tcp/tcp.hpp"

namespace server {

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
    private:
        std::string status;
        int statusCode;
        std::string protocolType;
        std::unordered_map<std::string, std::string> headers;
        int contentLength;

        tcp::TCPConn conn;
    public:
        Response(tcp::TCPConn& conn): conn(conn){}
        ~Response() = default;

        void write(const std::string& body) {
            contentLength = static_cast<int>(body.size());
            headers["Content-Length"] = std::to_string(contentLength);

            std::ostringstream response;

            response << protocolType << " " << statusCode << " " << status << "\r\n";

            for (const auto& [key, value] : headers) {
                response << key << ": " << value << "\r\n";
            }

            response << "\r\n" << body;

            conn.write(response.str());
        }

        void writeStatuses(int& statusCode, std::string& status) {
            statusCode = statusCode;
            status = status;
        }

        void setHeaders(std::string& key, std::string& value) {
            headers[key] = value;
        }

        void setProtocol(std::string& protocol) {
            protocolType = protocol;
        }

        void close() {
            conn.closeConn();
        }
};

}
