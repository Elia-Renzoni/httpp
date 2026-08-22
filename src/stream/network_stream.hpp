
#pragma once

#include <string>
#include <netinet/in.h>
#include <stdexcept>
#include <utility>

namespace stream {

class NetworkError : public std::runtime_error {
    public:
        explicit NetworkError(const std::string& exceptionMessage) : std::runtime_error("Network Error: " +  exceptionMessage) {};
};

class NetworkStream {
    protected:
        NetworkStream(std::string& addr, int& port): listenerAddress(addr), listenerPort(port), receiveBufferMaxSize(5048), readTimeout(10), writeTimeout(10){};
        NetworkStream(std::string& addr, int& port, int& customRecvBufferSize): listenerAddress(addr), listenerPort(port), receiveBufferMaxSize(customRecvBufferSize), readTimeout(10), writeTimeout(10){};
        NetworkStream(std::string& addr, int& port, int&customRecvBufferSize, int& readTimeout, int& writeTimeout): listenerAddress(addr), listenerPort(port), receiveBufferMaxSize(customRecvBufferSize), readTimeout(readTimeout), writeTimeout(writeTimeout){};
        NetworkStream(std::string& addr, int& port, int& readTimeout, int& writeTimeout): listenerAddress(addr), listenerPort(port), readTimeout(readTimeout), writeTimeout(writeTimeout){};
        ~NetworkStream() = default;

        void closeTCP();
        void bindAndlistenTCP();
        std::pair<char*,std::pair<ssize_t, int>> acceptTCP();
    private:
        void setSocketTimeoutOptions(int clientFd) {
            struct timeval rTimeOptions;
            struct timeval wTimeOptions;

            rTimeOptions.tv_sec = readTimeout;
            rTimeOptions.tv_usec = 0;
            wTimeOptions.tv_sec = writeTimeout;
            wTimeOptions.tv_usec = 0;

            if (setsockopt(clientFd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&rTimeOptions, sizeof(rTimeOptions)) < 0) {
                throw NetworkError("unable to set a read timeout for the underlying TCP connection");
            }

            if (setsockopt(clientFd, SOL_SOCKET, SO_SNDTIMEO, (const char*)&wTimeOptions, sizeof(wTimeOptions)) < 0) {
                throw NetworkError("unable to set a write timeout for the underlying TCP connection");
            }
        }
        std::string listenerAddress;
        int listenerPort;
        int socketFileDescriptor;
        int receiveBufferMaxSize;
        int readTimeout; // 10s default timeout
        int writeTimeout; // 10s deault timeout

        struct sockaddr_in serverAddress = {0};
};

}
