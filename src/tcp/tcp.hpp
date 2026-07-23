
#include <string>

#pragma once

namespace tcp {

class TCPConn {
    public:
        TCPConn(int sfd): socketFileDescriptor(sfd) {};
        ~TCPConn() = default;
        char* readUntil();
        void write(const std::string& data);
        void closeConn();
    private:
        int socketFileDescriptor;
};

}
