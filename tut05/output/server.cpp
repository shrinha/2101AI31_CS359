#include<bits/stdc++.h>
#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    // Create a socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Error creating socket." << std::endl;
        return 1;
    }

    // Bind the socket to a specific address and port
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(12345);

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Error binding socket." << std::endl;
        close(serverSocket);
        return 1;
    }

    // Listen for incoming connections
    if (listen(serverSocket, 1) == -1) {
        std::cerr << "Error listening for connections." << std::endl;
        close(serverSocket);
        return 1;
    }

    std::cout << "Server is listening..." << std::endl;

    while (true) {
        // Wait for a connection
        std::cout << "Waiting for a connection..." << std::endl;
        struct sockaddr_in clientAddress;
        socklen_t clientAddressSize = sizeof(clientAddress);
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressSize);
        if (clientSocket == -1) {
            std::cerr << "Error accepting connection." << std::endl;
            continue;
        }

        // Receive data from the client
        char buffer[1024];
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived == -1) {
            std::cerr << "Error receiving data from client." << std::endl;
            close(clientSocket);
            continue;
        }
        buffer[bytesReceived] = '\0';
        std::string data(buffer);

        std::cout << "Received: " << data << std::endl;

        std::string response;

        if (data.find("Task 1") == 0) {
            // Task 1: Echo back a message
            response = "Have a good day";
        } else if (data.find("Task 2") == 0) {
            // Task 2: Count vowels in a string
            size_t pos = data.find(":");
            if (pos != std::string::npos) {
                std::string text = data.substr(pos + 1);
                int vowels = 0;
                for (char c : text) {
                    if (std::string("aeiouAEIOU").find(c) != std::string::npos) {
                        vowels++;
                    }
                }
                response = "Number of vowels: " + std::to_string(vowels);
            } else {
                response = "Invalid task";
            }
        } else if (data.find("Task 3") == 0) {
            // Task 3: Check if a string is a palindrome
            size_t pos = data.find(":");
            if (pos != std::string::npos) {
                std::string text = data.substr(pos + 1);
                bool isPalindrome = true;
                for (size_t i = 0; i < text.length() / 2; i++) {
                    if (text[i] != text[text.length() - i - 1]) {
                        isPalindrome = false;
                        break;
                    }
                }
                if (isPalindrome) {
                    response = "String is palindrome";
                } else {
                    response = "String is not palindrome";
                }
            } else {
                response = "Invalid task";
            }
        } else if (data.find("Task 4") == 0) {
            // Task 4: Receive an array of integers and calculate their sum
            size_t pos = data.find(":");
            if (pos != std::string::npos) {
                std::string numbersStr = data.substr(pos + 1);
                std::istringstream iss(numbersStr);
                int num;
                int sum = 0;
                while (iss >> num) {
                    sum += num;
                }
                response = "Sum of elements: " + std::to_string(sum);
            } else {
                response = "Invalid task";
            }
        } else {
            response = "Invalid task";
        }

        // Send the response back to the client
        send(clientSocket, response.c_str(), response.length(), 0);

        // Close the connection
        close(clientSocket);
    }

    // Close the server socket
    close(serverSocket);

    return 0;
}
