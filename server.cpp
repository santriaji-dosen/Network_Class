#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

class Server {
public:
    Server(int port) : port(port), server_fd(0), new_socket(0) {}
    
    void start() {
        // Create socket
        server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd == 0) {
            std::cerr << "Socket creation failed!" << std::endl;
            return;
        }
        
        sockaddr_in address;
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);
        
        // Bind socket to port
        if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
            std::cerr << "Bind failed!" << std::endl;
            return;
        }
        
        // Listen for incoming connections
        if (listen(server_fd, 3) < 0) {
            std::cerr << "Listen failed!" << std::endl;
            return;
        }
        
        std::cout << "Server is listening on port " << port << "..." << std::endl;
        
        int addrlen = sizeof(address);
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        if (new_socket < 0) {
            std::cerr << "Accept failed!" << std::endl;
            return;
        }
        
        // Receive and respond to message
        char buffer[1024] = {0};
        read(new_socket, buffer, 1024);
        std::cout << "Client: " << buffer << std::endl;
        
        std::string response = "Message received!";
        send(new_socket, response.c_str(), response.size(), 0);
        
        close(new_socket);
    }
    
private:
    int port;
    int server_fd, new_socket;
};

int main() {
    Server server(9090);
    server.start();
    return 0;
}
