
#include <string>
#include "../stream/network_stream.hpp"

namespace server {
 
class Http : public stream::NetworkStream {
    public:
        Http(std::string& addr, int port): address(addr), port(port), maxHeaderBytes(16384), stream::NetworkStream(addr, port) {};
        Http(std::string addr, int port, int maxHeaderBytes): address(addr), port(port), maxHeaderBytes(maxHeaderBytes), stream::NetworkStream(addr, port){};
        ~Http() = default;

        void listenAndServe();
    private:
        std::string address;
        int port;
        size_t  maxHeaderBytes; // 16kb as a default value
};

}
