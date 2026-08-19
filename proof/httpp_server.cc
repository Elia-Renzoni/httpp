#include "../src/server/http_server.hpp"

void processRequest(server::Request req, server::Response& res) {
    std::string protoVersion = "HTTP/1.1"; 
    std::string headerKey = "Content-Type";  
    std::string headerValue = "text/plain";

    int status200 = 200;
    std::string okMsg = "OK";
    int status400 = 400;
    std::string badReqMsg = "Bad Request";

    res.setProtocol(protoVersion);
    res.setHeaders(headerKey, headerValue);

    if (req.methodType != "POST" || req.body != "Ping") {
        res.writeStatuses(status400, badReqMsg);
        std::string errResp = "Error Message";
        res.write(errResp);
    } else {
        res.writeStatuses(status200, okMsg);
        std::string okResp = "Pong";
        res.write(okResp);
    }
    
    res.close();
}

void handler(server::Request req, server::Response& res) {
    processRequest(req, res);
}

void handler2(server::Request req, server::Response& res) {
    processRequest(req, res);
}

int main() {
    std::string addr = "127.0.0.1";
    int listenPort = 8080;

    server::Http instance = server::Http(addr, listenPort);
    instance.handleFunc("/proof", handler);
    instance.handleFunc("/proof-again", handler2);

    instance.listenAndServe();
}
