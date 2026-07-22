
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "network_stream.hpp"

namespace stream { 

void NetworkStream::closeTCP() {
    close(socketFileDescriptor);
};

void NetworkStream::bindAndlistenTCP() {
    socketFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    auto opt = 1;
    setsockopt(socketFileDescriptor, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(listenerPort);
    if (inet_pton(AF_INET, listenerAddress.c_str(), &serverAddress.sin_addr) != 1) {
        close(socketFileDescriptor);
        throw NetworkError("invalid IP address " + listenerAddress);
    }

    auto result = bind(socketFileDescriptor, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    if (result < 0) {
        close(socketFileDescriptor);
        throw NetworkError("bind failed for " + listenerAddress);
    }

    auto opResult = listen(socketFileDescriptor, SOMAXCONN);
    if (opResult == -1) {
        close(socketFileDescriptor);
        throw NetworkError("failed to start TCP listener");
    }
};

std::pair<char*, std::pair<ssize_t, int>> NetworkStream::acceptTCP() {
    struct sockaddr_in client;
    socklen_t len = sizeof(client);
    auto sock = accept(socketFileDescriptor, (struct sockaddr *)&client, &len);
    if (sock < 0) {
         close(socketFileDescriptor);
         throw NetworkError("accept failed");
    }
    char* recvBuffer = new char[5048];
    ssize_t recvLen = read(sock, recvBuffer, 5048);
    if (recvLen == -1) {
         close(socketFileDescriptor);
         throw NetworkError("failed to read data");
    }

    if (recvLen < 5048) 
        recvBuffer[recvLen] = '\0';
    else
        recvBuffer[5047] = '\0';
    return {recvBuffer, {recvLen, socketFileDescriptor}};
}

}
