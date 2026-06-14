
#include <sys/socket.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "network_stream.hpp"

namespace stream { 

void NetworkStream::closeTCP() {
    close(socketFileDescriptor);
}

void NetworkStream::setupTCP() {
    socketFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    auto opt = 1;
    setsockopt(socketFileDescriptor, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    auto flags = fcntl(socketFileDescriptor, F_GETFL, 0);
    fcntl(socketFileDescriptor, F_SETFL, flags | O_NONBLOCK);
}

void NetworkStream::bindTCP() {
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(listenerPort);
    if (inet_pton(AF_INET, listenerAddress.c_str(), &serverAddress.sin_addr) != 1) {
        closeTCP();
        throw NetworkError("invalid IP address " + listenerAddress);
    }

    auto result = bind(socketFileDescriptor, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    if (result < 0) {
        closeTCP();
        throw NetworkError("bind failed for " + listenerAddress);
    }
}

void NetworkStream::listenTCP() {
    auto opResult = listen(socketFileDescriptor, SOMAXCONN);
    if (opResult == -1) {
        closeTCP();
        throw NetworkError("failed to start TCP listener");
    }
}

char* NetworkStream::acceptTCP() {
    struct sockaddr_in client;
    socklen_t len = sizeof(client);
    auto sock = accept(socketFileDescriptor, (struct sockaddr *)&client, &len);
    if (sock < 0) {
         closeTCP();
         throw NetworkError("accept failed");
    }
    char* recvBuffer = new char[5048];
    ssize_t recvLen = read(sock, recvBuffer, 5048);
    if (recvLen == -1) {
         closeTCP();
         throw NetworkError("failed to read data");
    }
    return recvBuffer;
}

}
