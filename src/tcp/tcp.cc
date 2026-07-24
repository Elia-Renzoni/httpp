#include <unistd.h>
#include <sys/socket.h>
#include <iostream>

#include "tcp.hpp"

namespace tcp {

std::pair<char*, ssize_t> TCPConn::readInitialBuffer() {
    return {initialBuffer, initialBufferLen};
};

std::pair<char*, ssize_t> TCPConn::readUntil() {
    char *streamBuffer = new char[2048];
    auto len = read(socketFileDescriptor, streamBuffer, 2048);
    if (len <= -1 || len == 0) {
        close(socketFileDescriptor);
        throw std::runtime_error("something went wrong while reading data from TCP");
    }
    return {streamBuffer, len};
};

void TCPConn::write(const std::string& data) {
    if (data.empty()) throw std::runtime_error("cannot send empty data");
    const char *dataToSend = data.c_str();

    auto result = send(socketFileDescriptor, dataToSend, sizeof(dataToSend), 0);
    if (result == 0 || result <= -1)
        close(socketFileDescriptor);
};


void TCPConn::closeConn() {
    close(socketFileDescriptor);
};
   
}
