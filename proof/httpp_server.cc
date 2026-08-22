#include "../src/server/http_server.hpp"

void processRequest(server::Request req, server::Response& res) {
    res.setHeaders("Content-Type", "text/plain");

    if (req.methodType != "POST" || req.body != "Ping") {
        res.writeStatus(server::StatusBadRequest);
        std::string errResp = "Error Message";
        res.write(errResp);
    } else {
        res.writeStatus(server::StatusOK);
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

void handler3(server::Request req, server::Response& res) {
    if (req.methodType != "GET") {
        res.setHeaders("Content-Type", "text/plain");
        res.writeStatus(server::StatusBadRequest);
        std::string errResp = "Error Message";
        res.write(errResp);
        res.close();
        return;
    }

    auto idValue = req.queryParameters["id"];
    auto nameValue = req.queryParameters["name"];
    auto postValue = req.queryParameters["post"];

    if (idValue != "12" || nameValue != "foo" || postValue != "mock bar") {
        res.setHeaders("Content-Type", "text/plain");
        res.writeStatus(server::StatusBadRequest);
        std::string errResp = "12 and foo, are you serious?";
        res.write(errResp);
        res.close();
        return;
    }

    res.setHeaders("Content-Type", "text/plain");
    res.writeStatus(server::StatusOK);
    std::string okResp = "Test Passed";
    res.write(okResp);
    res.close();
}

int main() {
    std::string addr = "127.0.0.1";
    int listenPort = 8080;

    server::Http instance = server::Http(addr, listenPort);
    instance.handleFunc("/proof", handler);
    instance.handleFunc("/proof-again", handler2);
    instance.handleFunc("/test", handler3);

    instance.listenAndServe();
}
