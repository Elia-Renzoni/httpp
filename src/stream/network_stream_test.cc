#include "network_stream.hpp"
#include <gtest/gtest.h>
#include <string>
#include <thread>
#include <chrono> // For std::this_thread::sleep_for
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream> // For std::cerr

namespace stream {

class TestableNetworkStream : public NetworkStream {
public:
    TestableNetworkStream(std::string& addr, int& port) : NetworkStream(addr, port) {}

    // Expose protected methods as public for testing
    using NetworkStream::bindAndlistenTCP;
    using NetworkStream::acceptTCP;
    using NetworkStream::closeTCP;
};

// Helper function to simulate a TCP client
void tcp_client_send(const std::string& ip_address, int port, const std::string& message) {
    int sock = 0;
    struct sockaddr_in serv_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        return;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, ip_address.c_str(), &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        close(sock);
        return;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection Failed");
        close(sock);
        return;
    }

    send(sock, message.c_str(), message.length(), 0);
    close(sock);
}

TEST(NetworkStreamTest, TCPServerReceivesMessage) {
    std::string test_address = "127.0.0.1";
    int test_port = 8080;
    std::string expected_message = "Hello World";

    TestableNetworkStream server(test_address, test_port);
    try {
        server.bindAndlistenTCP();
    } catch (const NetworkError& e) {
        std::cerr << "server error: " << e.what() << std::endl;
        FAIL();
    }

    std::string received_message;

    // Run acceptTCP in a separate thread
    std::thread server_thread([&]() {
        try {
            std::pair<char*, std::pair<ssize_t, int>> connPair = server.acceptTCP();
            char* buffer = connPair.first;
            ssize_t len = connPair.second.first;
            if (buffer) {
                received_message = std::string(buffer, len);
                delete[] buffer; // Free the dynamically allocated buffer
            }
        } catch (const NetworkError& e) {
            std::cerr << "server thread error: " << e.what() << std::endl;
        }
    });

    // Give the server a moment to start listening
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // Client sends a message
    tcp_client_send(test_address, test_port, expected_message);

    server_thread.join();

    // Verify the message
    ASSERT_EQ(received_message.substr(0, expected_message.length()), expected_message);

    server.closeTCP();
}

} // namespace stream
