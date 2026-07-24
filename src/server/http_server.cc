
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

}
