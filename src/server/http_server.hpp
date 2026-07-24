
#include <string>
#include <vector>
#include "../stream/network_stream.hpp"
#include "../tcp/tcp.hpp"

namespace server {
 
class Http : public stream::NetworkStream {
    public:
        Http(std::string& addr, int port): address(addr), port(port), maxHeaderBytes(16384), stream::NetworkStream(addr, port) {};
        Http(std::string addr, int port, int maxHeaderBytes): address(addr), port(port), maxHeaderBytes(maxHeaderBytes), stream::NetworkStream(addr, port){};
        Http(std::string addr, int port, int maxHeaderBytes, int customRecvBufferSSize): address(addr), port(port), maxHeaderBytes(maxHeaderBytes), stream::NetworkStream(addr, port){};

        ~Http() = default;

        void listenAndServe();
    private:
        void handlePacket(tcp::TCPConn& conn) {
            std::vector<char> mergedChunks;
            std::vector<char> scanStack;

            std::pair<char*, ssize_t> buffer = conn.readInitialBuffer();
            char *data = buffer.first;
            ssize_t totalBytes = buffer.second;
            if (isHeaderReached(data, totalBytes)) {
                // TODO-> how to respond?
            }

            do {
                buffer = conn.readUntil();
                data = buffer.first;
                totalBytes = buffer.second;


            } while (isHeaderReached(data, totalBytes));
            
        };

        bool isHeaderReached(char *data, ssize_t totalBytes) {
            std::vector<char> scanStack;

            for (ssize_t offset = 0; offset < totalBytes; offset++) {
                scanStack.push_back(data[offset]);
                if (scanStack.size() < 4) {
                    continue;
                }

                char nextShouldBe = '\r';
                for (auto it = scanStack.begin(); it != scanStack.end(); it++) {
                    switch (*it) {
                        case '\r':
                            if (nextShouldBe == '\r') {
                                nextShouldBe = '\n';
                                break;
                            } else {
                                return false;
                            }
                        case '\n':
                            if (nextShouldBe == '\n') {
                                nextShouldBe = '\r';
                                break;
                            } else { 
                                return false;
                            }
                        default:
                            scanStack.clear();
                            return false;
                    }
                    
                }
            }
            return false;
        };

        std::string address;
        int port;
        size_t  maxHeaderBytes; // 16kb as a default value
};

}
