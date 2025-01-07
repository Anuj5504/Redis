#include <iostream>
#include <string>
#include <winsock2.h>
#include "../encoder/resp_encoder.h"

#pragma comment(lib, "ws2_32.lib")

const char* SERVER_IP = "127.0.0.1";  
const int SERVER_PORT = 6379;         // Default port

// Initialize Winsock
bool initializeWinsock() {
    WSADATA wsa;
    int result = WSAStartup(MAKEWORD(2, 2), &wsa);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << std::endl;
        return false;
    }
    return true;
}

// Cleanup Winsock
void cleanupWinsock() {
    WSACleanup();
}

// Connect to the server
SOCKET createClientSocket() {
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed!" << std::endl;
        cleanupWinsock();
        return INVALID_SOCKET;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);  // Convert IP address to binary
    serverAddr.sin_port = htons(SERVER_PORT);           // Port number

    int result = connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    if (result == SOCKET_ERROR) {
        std::cerr << "Connection failed!" << std::endl;
        closesocket(clientSocket);
        cleanupWinsock();
        return INVALID_SOCKET;
    }

    std::cout << "Connected to the server!" << std::endl;
    return clientSocket;
}

// Send a command to the server and receive a response
bool sendCommand(SOCKET clientSocket, const std::string& command) {
    int result = send(clientSocket, command.c_str(), command.length(), 0);
    if (result == SOCKET_ERROR) {
        std::cerr << "Send failed!" << std::endl;
        return false;
    }

    // Receive response from the server
    char buffer[1024] = {0};
    result = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (result > 0) {
        std::cout << "Server response: " << buffer << std::endl;
    } else if (result == 0) {
        std::cout << "Server disconnected!" << std::endl;
        return false;  // Server closed the connection
    } else {
        std::cerr << "Receive failed!" << std::endl;
        return false;
    }
    return true;
}

// Main client loop
int main() {
    if (!initializeWinsock()) {
        return -1;
    }

    SOCKET clientSocket = createClientSocket();
    if (clientSocket == INVALID_SOCKET) {
        return -1;
    }

    std::string command;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, command);
        Encoder encoder;

        // Trim leading and trailing spaces
        command.erase(0, command.find_first_not_of(' '));
        command.erase(command.find_last_not_of(' ') + 1);

        if (command.empty()) {
            continue; // Skip empty commands
        }

        std::vector<std::string> commandArray=encoder.convertIntoArray(command);

        std::string encodedCommand = encoder.encodeArray(commandArray);

        if (command == "exit") {
            std::cout << "Exiting..." << std::endl;
            sendCommand(clientSocket, encodedCommand);
            break; 
        }

        if (!sendCommand(clientSocket, encodedCommand)) {
            std::cout<<"Server disconnected!"<<std::endl;
            break; 
        }
    }

    // Clean up resources 
    closesocket(clientSocket);
    cleanupWinsock();

    return 0;
}
