
#include <string>
#include "../stream/network_stream.hpp"

namespace server {
 
class Http : public stream::NetworkStream {
    public:
        Http();
        Http(std::string& addr, int& port, int& maxHeaderBytes);
        ~Http() = default;

        void listenAndServe();
    private:
        std::string address;
        int port;
        int maxHeaderBytes; // 16kb as a default value
};

}
