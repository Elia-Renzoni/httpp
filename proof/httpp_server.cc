

#include "../src/server/http_server.hpp"

void handler(Request req, Response& res) {

}

int main() {
    std::string addr = "127.0.0.1";
    int listenPort = 8080;

    server::Http instance = server::Http(addr, listenPort);
    instance.handleFunc("/proof", handler);
    instance.listenAndServe();
}
