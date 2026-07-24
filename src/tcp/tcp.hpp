
#include <string>

#pragma once

namespace tcp {

class TCPConn {
    public:
        TCPConn(int sfd, char* initialBuffer): socketFileDescriptor(sfd), initialBuffer(initialBuffer) {};
        ~TCPConn() = default;
        char* readUntil();
        void write(const std::string& data);
        void closeConn();
        char *readInitialBuffer();
    private:
        int socketFileDescriptor;
        char *initialBuffer; // initialBuffer represent the first data readed
};

}
