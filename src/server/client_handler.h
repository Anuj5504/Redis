#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

#include <string>
#include <winsock2.h>

class ClientHandler {
public:
    ClientHandler(SOCKET clientSocket);
    ~ClientHandler();

    void setNonBlocking(SOCKET sock);
    void handle();

private:
    SOCKET clientSocket;
    void processCommand(const std::string& command);

};

#endif // CLIENT_HANDLER_H
