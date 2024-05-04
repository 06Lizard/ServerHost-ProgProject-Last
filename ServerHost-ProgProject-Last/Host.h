#pragma once

#include <iostream>
#include <winsock2.h>

class Host {
private:
    // General
    const short PORT;

    // Host
    WSADATA wsaData;
    SOCKET serverSocket;
    sockaddr_in serverAddr;

    // Receive Message
    sockaddr_in clientAddr;
    SOCKET clientSocket;

public:
    Host();
    ~Host();

private:
    int StartWSA();
    int CreateSocket();
    int Listen(int clientLim);
    int ConnectClient();
    int ReceiveMessage();
    void CloseConnection();
};
