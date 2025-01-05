#include <iostream>
#include <winsock2.h>
#include <thread>
#include <vector>
#include "client_handler.h"

#pragma comment(lib, "ws2_32.lib")

const int PORT = 6379;
const int MAX_CLIENTS = 10;

void handleClient(SOCKET clientSocket){
    ClientHandler handler(clientSocket);
    handler.handle();
}

int main()
{
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        std::cerr << "Failed to initialize Winsock\n";
        return -1;
    }

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET)
    {
        std::cerr << "Failed to create socket\n";
        WSACleanup();
        return -1;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (bind(serverSocket, (sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        std::cerr << "Bind failed with error: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }

    if (listen(serverSocket, MAX_CLIENTS) == SOCKET_ERROR)
    {
        std::cerr << "Listen failed with error: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }

    std::cout << "Server started on port " << PORT << std::endl;
    std::vector<std::thread> clientThreads;
    while (true)
    {
        sockaddr_in clientAddr{};
        int clientAddrSize = sizeof(clientAddr);
        SOCKET clientSocket = accept(serverSocket, (sockaddr *)&clientAddr, &clientAddrSize);

        if (clientSocket == INVALID_SOCKET)
        {
            std::cerr << "Accept failed with error: " << WSAGetLastError() << std::endl;
            continue;
        }

        std::cout << "Client connected: " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << std::endl;

        std::thread clientThread(handleClient, clientSocket);

        clientThreads.push_back(std::move(clientThread));
    }

    for (auto &t : clientThreads)
    {
        if (t.joinable())
        {
            t.join();
        }
    }

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
