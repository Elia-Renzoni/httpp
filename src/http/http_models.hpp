#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <sstream>

#include "../tcp/tcp.hpp"

namespace server {

enum StatusList {
    StatusOK = 200,
    StatusCreated = 201,
    StatusNoContent = 204,
    StatusMovedPermanently = 301,
    StatusFound = 302,
    StatusNotModified = 304,
    StatusBadRequest = 400,
    StatusUnauthorized = 401,
    StatusForbidden = 403,
    StatusNotFound = 404,
    StatusMethodNotAllowed = 405,
    StatusConflict = 409,
    StatusTooManyRequests = 429,
    StatusInternalServerError = 500,
    StatusBadGateway = 502,
    StatusServiceUnavailable = 503,
    StatusGatewayTimeout = 504
};

inline constexpr std::string_view MethodGet = "GET";
inline constexpr std::string_view MethodPost = "POST";
inline constexpr std::string_view MethodPut = "PUT";
inline constexpr std::string_view MethodPatch = "PATCH";
inline constexpr std::string_view MethodDelete = "DELETE";
inline constexpr std::string_view MethodHead = "HEAD";
inline constexpr std::string_view MethodOptions = "OPTIONS";
inline constexpr std::string_view MethodConnect = "CONNECT";
inline constexpr std::string_view MethodTrace = "TRACE";

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
        std::unordered_map<std::string, std::string> headers;
        int contentLength;

        tcp::TCPConn conn;
        std::unordered_map<int, std::string> statusTable = {
            {StatusOK, "OK"},
            {StatusCreated, "Created"},
            {StatusNoContent, "No Content"},
            {StatusMovedPermanently, "Moved Permanently"},
            {StatusFound, "Found"},
            {StatusNotModified, "Not Modified"},
            {StatusBadRequest, "Bad Request"},
            {StatusUnauthorized, "Unauthorized"},
            {StatusForbidden, "Forbidden"},
            {StatusNotFound, "Not Found"},
            {StatusMethodNotAllowed, "Method Not Allowed"},
            {StatusConflict, "Conflict"},
            {StatusTooManyRequests, "Too Many Requests"},
            {StatusInternalServerError, "Internal Server Error"},
            {StatusBadGateway, "Bad Gateway"},
            {StatusServiceUnavailable, "Service Unavailable"},
            {StatusGatewayTimeout, "Gateway Timeout"}
        };

    public:
        Response(tcp::TCPConn& conn): conn(conn){}
        ~Response() = default;

        void write(const std::string& body) {
            contentLength = static_cast<int>(body.size());
            headers["Content-Length"] = std::to_string(contentLength);

            std::ostringstream response;

            response << "HTTP/1.1" << " " << statusCode << " " << status << "\r\n";

            for (const auto& [key, value] : headers) {
                response << key << ": " << value << "\r\n";
            }

            response << "\r\n" << body;

            conn.write(response.str());
        }

        void writeStatus(int statusCode) {
            this->statusCode = statusCode;
            if (!statusTable.count(statusCode)) {
                throw std::runtime_error("httpp does not support the given status code");
            }
            this->status = statusTable[statusCode];
        }

        void setHeaders(std::string key, std::string value) {
            headers[key] = value;
        }

        void close() {
            conn.closeConn();
        }
};

}
