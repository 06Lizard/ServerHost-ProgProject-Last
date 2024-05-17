#pragma once
#include <winsock2.h>
#include <iostream>
#include <string>
#include <future>
#include "ClientManager.h"

class ClientHandler {
private:
    SOCKET clientSocket;
    std::string save;
    std::string clientId;
    ClientManager* clientManager;
    std::atomic<bool> running;

public:
    ClientHandler(SOCKET socket, ClientManager* manager);
    ~ClientHandler();

    void ReceiveMSG();
    std::future<int> SendMSG(std::string msg, SOCKET rcp);
    std::future<int> LogMSG(std::string msg);

private:
    void ProcessMessage(const std::string& msg);
};