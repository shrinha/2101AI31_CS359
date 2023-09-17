#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    // Create a socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Error creating socket." << std::endl;
        return 1;
    }

    // Define the server address and port
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(12345);
    if (inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr) <= 0) {
        std::cerr << "Invalid server address." << std::endl;
        close(clientSocket);
        return 1;
    }

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Error connecting to server." << std::endl;
        close(clientSocket);
        return 1;
    }

    // Send a message to the server
    std::string message = "Task 2:You must be the change you wish to see in the world";
    if (send(clientSocket, message.c_str(), message.length(), 0) == -1) {
        std::cerr << "Error sending message to server." << std::endl;
        close(clientSocket);
        return 1;
    }

    // Receive a response from the server
    char buffer[1024];
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesReceived == -1) {
        std::cerr << "Error receiving response from server." << std::endl;
        close(clientSocket);
        return 1;
    }
    buffer[bytesReceived] = '\0';
    std::string response(buffer);

    std::cout << "Server Response: " << response << std::endl;

    // Close the client socket
    close(clientSocket);

    return 0;
}
