
#include <string>
#include <netinet/in.h>
#include <stdexcept>
#include <utility>

#pragma once

namespace stream {

class NetworkError : public std::runtime_error {
    public:
        explicit NetworkError(const std::string& exceptionMessage) : std::runtime_error("Network Error: " +  exceptionMessage) {};
};

class NetworkStream {
    protected:
        NetworkStream(std::string& addr, int& port): listenerAddress(addr), listenerPort(port){};

        void setupTCP();
        void listenTCP();
        void bindTCP();
        std::pair<char*,std::pair<ssize_t, int>> acceptTCP();
        void closeTCP();
    private:
        std::string listenerAddress;
        int listenerPort;
        int socketFileDescriptor;

        struct sockaddr_in serverAddress = {0};
};

}
