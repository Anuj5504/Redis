#include "client_handler.h"
#include "../decoder/resp_decoder.h"
#include <iostream>
#include <cstring>
#include <unordered_map>

void ClientHandler::setNonBlocking(SOCKET sock)
{
    u_long mode = 0;
    ioctlsocket(sock, FIONBIO, &mode);
}

ClientHandler::ClientHandler(SOCKET clientSocket) : clientSocket(clientSocket) {}

ClientHandler::~ClientHandler()
{
    closesocket(clientSocket);
}


void ClientHandler::handle()
{
    const int BufferSize = 1024;
    char buffer[BufferSize];
    int bytesRead;
    setNonBlocking(clientSocket);

    while (true)
    {
        memset(buffer, 0, BufferSize);
        bytesRead = recv(clientSocket, buffer, BufferSize, 0); // Receive data from the client

        if (bytesRead > 0)
        {
            std::string command(buffer, 0, bytesRead);
            std::cout << "Received command: " << command << std::endl;

            // Strip any newline characters if present
            if (command.back() == '\n')
            {
                command.pop_back();
            }
            if (command.back() == '\r')
            {
                command.pop_back();
            }

            processCommand(command);
        }
        else if (bytesRead == 0)
        {
            std::cout << "Client disconnected" << std::endl;
            break;
        }
        else if (bytesRead == SOCKET_ERROR)
        {
            if (WSAGetLastError() == WSAEWOULDBLOCK)
            {
                // No data available, continue the loop
                continue;
            }
            std::cerr << "Error: " << WSAGetLastError() << std::endl;
            break;
        }
    }
}
std::unordered_map<std::string, std::string> hash;

void ClientHandler::processCommand(const std::string &command)
{

    Decoder decoder;
    std::vector<std::string> decodedarray = decoder.decodeCommand(command);
    std::string decodedCommand = decodedarray[0];

    std::string response;

    if (decodedCommand == "PING")
    {
        response = "PONG\r\n";
    }
    else if (decodedCommand == "SET")
    {
        hash[decodedarray[1]] = decodedarray[2];
        response = "Command processed\r\n";
    }
    else if (decodedCommand == "GET")
    {
        if (hash.find(decodedarray[1]) == hash.end())
        {
            response = "Key not found\r\n";
        }
        else
        {
            response = hash[decodedarray[1]] +"\r\n";
        }
    }
    else if (decodedCommand == "EXIT")
    {
        response = "Goodbye!\r\n";
        std::cout << "Closing socket after exit command." << std::endl;
        send(clientSocket, response.c_str(), response.size(), 0);

        // Close socket after sending the response
        if (closesocket(clientSocket) == SOCKET_ERROR)
        {
            std::cerr << "Failed to close socket: " << WSAGetLastError() << std::endl;
        }
        return; // Exit after closing the socket
    }
    else
    {
        response = "ERROR\r\n";
    }

    send(clientSocket, response.c_str(), response.size(), 0);
}
