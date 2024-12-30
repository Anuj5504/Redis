#include "client_handler.h"
#include <winsock2.h>
#include <iostream>
#include <thread>

void handleClient(SOCKET clientSocket) {
    ClientHandler handler(clientSocket);
    handler.handle();
}