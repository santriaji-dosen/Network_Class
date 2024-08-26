#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

class Client {
public:
    Client(const std::string& server_ip, int port) : server_ip(server_ip), port(port), sock(0) {}
    
    void start() {
        // Create socket
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) {
            std::cerr << "Socket creation failed!" << std::endl;
            return;
        }
        
        sockaddr_in serv_addr;
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(port);
        
        // Convert IPv4 and IPv6 addresses from text to binary form
        if(inet_pton(AF_INET, server_ip.c_str(), &serv_addr.sin_addr) <= 0) {
            std::cerr << "Invalid address/ Address not supported!" << std::endl;
            return;
        }
        
        // Connect to server
        if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
            std::cerr << "Connection Failed!" << std::endl;
            return;
        }
        
        // Send message to server
        std::string message = "Hello from client!";
        send(sock, message.c_str(), message.size(), 0);
        
        // Receive response
        char buffer[1024] = {0};
        read(sock, buffer, 1024);
        std::cout << "Server: " << buffer << std::endl;
        
        close(sock);
    }
    
private:
    std::string server_ip;
    int port;
    int sock;
};

int main() {
    Client client("127.0.0.1", 9090);
    client.start();
    return 0;
}
