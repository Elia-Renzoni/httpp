
#include <vector>
#include <string>

#pragma once

namespace tcp {

class TCPConn {
    public:
        TCPConn(char *buf, ssize_t size, int sfd): alreadyReadedBytes(buf), size(size), socketFileDescriptor(sfd) {};
        ~TCPConn() = default;
        char* readUntil();
        void write(const std::string& data);
        void closeConn();
        [[maybe_unused]]
        void collectData(char *fetchedData);
        [[maybe_unused]]
        char* getCollectedData();
    private:
        int socketFileDescriptor;
        [[maybe_unused]]
        char *alreadyReadedBytes;
        [[maybe_unused]]
        ssize_t size;
        [[maybe_unused]]
        std::vector<char*> collector;
};

}
