
#include "../src/server/http_server.hpp"

void handler(server::Request req, server::Response& res) {
    std::string protoVersion = "HTTP 1.1";
    std::string headerKey = "Content-Type"; 
    std::string headerValue = "text/plain";
    std::string badReq = "Bad Request";
    std::string ok = "Ok";
    int status400 = 400;
    int status200 = 200;

    if (req.methodType != "POST" && req.body != "Ping") {
        res.setProtocol(protoVersion);
        res.setHeaders(headerKey, headerValue);
        res.writeStatuses(status400, badReq);

        auto response = "Error Message";
        res.write(response);
        res.close();
        return;
    }

    res.setHeaders(headerKey, headerValue);
    res.setProtocol(protoVersion);
    res.writeStatuses(status200, ok);
    auto response = "Pong";
    res.write(response);
    res.close();
}

void handler2(server::Request req, server::Response& res) {
    std::string protoVersion = "HTTP 1.1";
    std::string headerKey = "Content-Type"; 
    std::string headerValue = "text/plain";
    std::string badReq = "Bad Request";
    std::string ok = "Ok";
    int status400 = 400;
    int status200 = 200;

    if (req.methodType != "POST" && req.body != "Ping") {
        res.setProtocol(protoVersion);
        res.setHeaders(headerKey, headerValue);
        res.writeStatuses(status400, badReq);

        auto response = "Error Message";
        res.write(response);
        res.close();
        return;
    }

    res.setHeaders(headerKey, headerValue);
    res.setProtocol(protoVersion);
    res.writeStatuses(status200, ok);
    auto response = "Pong";
    res.write(response);
    res.close();
}

int main() {
    std::string addr = "127.0.0.1";
    int listenPort = 8080;

    server::Http instance = server::Http(addr, listenPort);
    instance.handleFunc("/proof", handler);
    instance.handleFunc("/proof-again", handler2);

    instance.listenAndServe();
}
