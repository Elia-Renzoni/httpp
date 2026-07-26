
#include <string>
#include <vector>
#include <iostream>
#include "../stream/network_stream.hpp"
#include "../tcp/tcp.hpp"
#include "../parsing/parser.hpp"
#include "../parsing/scanner.hpp"

namespace server {
 
class Http : public stream::NetworkStream {
    public:
        Http(std::string& addr, int port): address(addr), port(port), maxHeaderBytes(16384), stream::NetworkStream(addr, port) {};
        Http(std::string addr, int port, int maxHeaderBytes): address(addr), port(port), maxHeaderBytes(maxHeaderBytes), stream::NetworkStream(addr, port){};
        Http(std::string addr, int port, int maxHeaderBytes, int customRecvBufferSSize): address(addr), port(port), maxHeaderBytes(maxHeaderBytes), stream::NetworkStream(addr, port){};

        ~Http() = default;

        void listenAndServe();
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

        void handleConnection(tcp::TCPConn& conn) {
            std::vector<char> mergedChunks;
            // counter for all the bytes readed per buffer
            ssize_t totalBytes;

            std::pair<char*, ssize_t> buffer = conn.readInitialBuffer();
            char *data = buffer.first;
            totalBytes +=  buffer.second;
            if (isHeaderReached(data, totalBytes)) {
                goto PARSE;
            }

            if (totalBytes < maxHeaderBytes) {
                mergedChunks.insert(mergedChunks.end(), data, data + totalBytes);
            } else {
                std::string response = buildHTTPResponse("431", "Request Header Fields Too Large", "");
                conn.write(response);
                conn.closeConn();
            }

            do {
                buffer = conn.readUntil();
                data = buffer.first;
                totalBytes += buffer.second;
                mergedChunks.insert(mergedChunks.end(), data, data + totalBytes);
            } while (!isHeaderReached(data, totalBytes) && totalBytes <= maxHeaderBytes);

            if (!isHeaderReached(mergedChunks.data(), totalBytes)) {
                std::string response = buildHTTPResponse("400", "Bad Request", "");
                conn.write(response);
                conn.closeConn();
            }
            
            if (totalBytes > maxHeaderBytes) {
                std::string response = buildHTTPResponse("431", "Request Header Fields Too Large", "");
                conn.write(response);
                conn.closeConn();
            }

PARSE:
            parsing::TokensManager tm = parsing::TokensManager();
            parsing::Scanner lexer = parsing::Scanner(tm, mergedChunks.data(), totalBytes);
            parsing::Parser parser = parsing::Parser(lexer);

            try {
                parser.parseRequestLine();
                parser.parseGenAndEntityHeader();
            } catch (parsing::ParserException& exp) {
                std::string response = buildHTTPResponse("400", "Bad Request", exp.what());
                conn.write(response);
                conn.closeConn();
            }

            // TODO-> handle endpoint rounting

        };

        bool isHeaderReached(char *data, ssize_t totalBytes) {
            if (totalBytes >= 4) {
                std::string_view view(data, totalBytes);
                size_t position = view.find("\r\n\r\n");
                if (position != std::string_view::npos)
                    return true;
            }

            return false;
        };

        std::string address;
        int port;
        size_t  maxHeaderBytes; // 16kb as a default value
};

}
