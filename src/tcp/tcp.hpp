
#include <string>

#pragma once

namespace tcp {

class TCPConn {
    public:
        TCPConn(int sfd, char* initialBuffer, ssize_t initBufferLen): socketFileDescriptor(sfd), initialBuffer(initialBuffer), initialBufferLen(initBufferLen) {};
        ~TCPConn() = default;
        std::pair<char*, ssize_t> readUntil();
        void write(const std::string& data);
        void closeConn();
        std::pair<char*, ssize_t> readInitialBuffer();
    private:
        int socketFileDescriptor;
        char *initialBuffer; // initialBuffer represent the first data readed
        ssize_t initialBufferLen;
};

}
