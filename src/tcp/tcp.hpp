
#include <vector>

#pragma once

namespace tcp {

class Connection {
    public:
        void read();
        void write();
        void closeReader();
        void closeWriter();
    private:
        int socketFileDescriptor;
        std::vector<char> buffer;
};

}
