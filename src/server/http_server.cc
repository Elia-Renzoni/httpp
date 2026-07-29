
#include "http_server.hpp"
#include "../tcp/tcp.hpp"

namespace server {

void Http::listenAndServe() {
    stream::NetworkStream::bindAndlistenTCP();

    for (;;) {
        std::pair<char*, std::pair<ssize_t, int>> conn = stream::NetworkStream::acceptTCP();
        tcp::TCPConn tcpConn(conn.second.second, conn.first, conn.second.first); // pass the socket file descriptor as parameter and also the first bytes readed

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
