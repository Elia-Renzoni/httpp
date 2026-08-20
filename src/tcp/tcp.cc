#include <unistd.h>
#include <sys/socket.h>
#include <iostream>
#include "../logger/logger.hpp"

#include "tcp.hpp"

namespace tcp {

std::pair<char*, ssize_t> TCPConn::readInitialBuffer() {
    return {initialBuffer, initialBufferLen};
};

std::pair<char*, ssize_t> TCPConn::readUntil() {
    std::string streamBuffer;
    char chunk[2048];

    ssize_t len = read(socketFileDescriptor, chunk, 2048);
    if (len < 0) {
            close(socketFileDescriptor);
            throw std::runtime_error("something went wrong while reading data from TCP");
    }

    if (len == 0) return {};
    streamBuffer.append(chunk);

    return {streamBuffer.data(), len};
};

void TCPConn::write(const std::string& data) {
    if (data.empty()) throw std::runtime_error("cannot send empty data");
    const char *dataToSend = data.c_str();
    ssize_t totalSize = data.size();

    auto result = send(socketFileDescriptor, dataToSend, totalSize, 0);
    if (result <= 0) {
        LOG_ERROR("TCP failure occured while sending data to the client");
        close(socketFileDescriptor);
    }
};


void TCPConn::closeConn() {
    close(socketFileDescriptor);
};
   
}
