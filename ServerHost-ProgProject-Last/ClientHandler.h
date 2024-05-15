#pragma once
#include <winsock2.h>
#include <iostream>
#include <string>
#include <future>

class ClientHandler {
private:
    SOCKET clientSocket;
    std::string save;
    std::atomic<bool> running;

public:
    ClientHandler(SOCKET socket);
    ~ClientHandler();

    void ReceiveMSG();
    std::future<int> SendMSG(std::string msg);
    std::future<int> LogMSG(std::string msg);
};