
#include <string>
#include <vector>
#include <unordered_map>
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
        void handleConnection(tcp::TCPConn& conn) {
            std::vector<char> mergedChunks;

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
            if (totalBytes >= 4) {
                std::string_view view(data, totalBytes);
                size_t position = view.find("\r\n\r\n");
                if (position != std::string_view::npos)
                    return true;
            }

            return false;
        };

        std::string address;
        int port;
        size_t  maxHeaderBytes; // 16kb as a default value
};

}
