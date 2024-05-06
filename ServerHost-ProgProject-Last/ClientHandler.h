#pragma once
#include <winsock2.h>
#include <iostream>

class ClientHandler {
private:
    SOCKET clientSocket;

public:
    ClientHandler(SOCKET socket);
    ~ClientHandler();

    void ReceiveMessages();
};