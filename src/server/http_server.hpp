
#include <string>
#include <vector>
#include <functional>
#include "../stream/network_stream.hpp"
#include "../tcp/tcp.hpp"
#include "../parsing/parser.hpp"
#include "../parsing/scanner.hpp"

namespace server {

using HttpHandler = std::function<void(Request, Response&)>;

class Http : public stream::NetworkStream {
    public:
        Http(std::string& addr, int port): address(addr), port(port), maxHeaderBytes(16384), stream::NetworkStream(addr, port) {};
        Http(std::string addr, int port, int maxHeaderBytes): address(addr), port(port), maxHeaderBytes(maxHeaderBytes), stream::NetworkStream(addr, port){};
        Http(std::string addr, int port, int maxHeaderBytes, int customRecvBufferSSize): address(addr), port(port), maxHeaderBytes(maxHeaderBytes), stream::NetworkStream(addr, port){};

        ~Http() = default;

        void listenAndServe();
        void handleFunc(std::string endpoint, HttpHandler handler);
    private:
        std::string buildHTTPResponse(std::string statusCode, std::string reason, std::string body) {
            std::string response =
                "HTTP/1.1 " + statusCode + " " + reason + "\r\n"
                "Content-Type: text/plain\r\n"
                "Content-Length: " + std::to_string(body.size()) + "\r\n"
                "Connection: close\r\n"
                "\r\n" + body;
            return response;
        }

        ssize_t findHeaderEndOffset(const char *data, ssize_t totalBytes) {
            if (totalBytes >= 4) {
                std::string_view view(data, totalBytes);
                size_t position = view.find("\r\n\r\n");
                if (position != std::string_view::npos) {
                    return static_cast<ssize_t>(position + 4);
                }
            }
            return -1;
        }

        void handleConnection(tcp::TCPConn& conn) {
            std::vector<char> mergedChunks;
            ssize_t totalBytes = 0;
            ssize_t headerEndOffset = -1;

            std::pair<char*, ssize_t> buffer = conn.readInitialBuffer();
            char *data = buffer.first;
            ssize_t bytesRead = buffer.second;

            if (bytesRead > 0) {
                totalBytes += bytesRead;
                mergedChunks.insert(mergedChunks.end(), data, data + bytesRead);
            }

            headerEndOffset = findHeaderEndOffset(mergedChunks.data(), totalBytes);
            if (headerEndOffset != -1) {
                goto PARSE;
            }

            if (totalBytes >= maxHeaderBytes) {
                std::string response = buildHTTPResponse("431", "Request Header Fields Too Large", "");
                conn.write(response);
                conn.closeConn();
                return;
            }

            do {
                buffer = conn.readUntil();
                data = buffer.first;
                bytesRead = buffer.second;

                if (bytesRead <= 0) break;

                totalBytes += bytesRead;
                mergedChunks.insert(mergedChunks.end(), data, data + bytesRead);

                headerEndOffset = findHeaderEndOffset(mergedChunks.data(), totalBytes);
            } while (headerEndOffset == -1 && totalBytes <= maxHeaderBytes);

            if (headerEndOffset == -1) {
                std::string response = buildHTTPResponse("400", "Bad Request", "");
                conn.write(response);
                conn.closeConn();
                return;
            }

            if (totalBytes > maxHeaderBytes) {
                std::string response = buildHTTPResponse("431", "Request Header Fields Too Large", "");
                conn.write(response);
                conn.closeConn();
                return;
            }

        PARSE:
            parsing::TokensManager tm = parsing::TokensManager();
            parsing::Scanner lexer = parsing::Scanner(tm, mergedChunks.data(), headerEndOffset);
            parsing::Parser parser = parsing::Parser(lexer);

            try {
                parser.parseRequestLine();
                parser.parseGenAndEntityHeader();
            } catch (parsing::ParserException& exp) {
                std::string response = buildHTTPResponse("400", "Bad Request", exp.what());
                conn.write(response);
                conn.closeConn();
                return;
            }

            Request req;
            parser.parserStack->walkStack(req);

            if (!(routeMap.count(req.endpoint))) {
                std::string response = buildHTTPResponse("400", "Bad Request", "function handler not found for " + req.endpoint);
                conn.write(response);
                conn.closeConn();
                return;
            }

            std::vector<char> bodyBuffer;

            if (totalBytes > headerEndOffset) {
                bodyBuffer.insert(bodyBuffer.end(), mergedChunks.begin() + headerEndOffset, mergedChunks.end());
            }

            if (req.contentLength > 0) {
                ssize_t remainingBytes = req.contentLength - bodyBuffer.size();

                while (remainingBytes > 0) {
                    buffer = conn.readUntil();
                    data = buffer.first;
                    bytesRead = buffer.second;

                    if (bytesRead <= 0) break;

                    bodyBuffer.insert(bodyBuffer.end(), data, data + bytesRead);
                    remainingBytes -= bytesRead;
                }

                req.body = std::string(bodyBuffer.begin(), bodyBuffer.end());
            }

            HttpHandler func = routeMap[req.endpoint];
            Response res(conn);

            func(req, res);
        };


        std::string address;
        int port;
        size_t  maxHeaderBytes; // 16kb as a default value
        std::unordered_map<std::string, HttpHandler> routeMap;
};

}
