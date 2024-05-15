#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <vector>

class SocketManager {
private:
    SOCKET serverSocket;
    sockaddr_in serverAddr;

public:
    SocketManager();
    ~SocketManager();

    bool Start(int port);
    SOCKET AcceptClient() const;
    void Close();
    SOCKET GetListeningSocket() const;
    std::vector<SOCKET> CheckEvents() const;
};