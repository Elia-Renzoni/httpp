
#include <thread>

#include "http_server.hpp"
#include "../tcp/tcp.hpp"
#include "../logger/logger.hpp"

namespace server {

void Http::listenAndServe() {
    stream::NetworkStream::bindAndlistenTCP();

    LOG_INFO("http server ready to accept connections...");
    for (;;) {
        std::pair<char*, std::pair<ssize_t, int>> conn = stream::NetworkStream::acceptTCP();

        LOG_INFO("tcp connection established");
        std::thread t(&Http::handleConnection, this, tcp::TCPConn(conn.second.second, conn.first, conn.second.first));
        t.detach();
    }
}

void Http::handleFunc(std::string endpoint, HttpHandler handler) {
    if (endpoint.empty()) {
        throw std::runtime_error("cannot add an HTTP handler with an empty endpoint");
    }

    if (routeMap.count(endpoint)) {
        throw std::runtime_error("endpoint target already present");
    }

    routeMap[endpoint] = handler;
}

}
