#pragma once
#include <winsock2.h>
#include <iostream>
#include <string>
#include <future>
#include "ClientManager.h"
#include "Colors.h"

class ClientHandler : public Colors
{
private:
    int idx;
    std::string save;
    std::string clientId;
    ClientManager* clientManager;
    std::atomic<bool> running;

public:
    ClientHandler(int idx, ClientManager* manager);
    ~ClientHandler();

    void ReceiveMSG();
    std::future<int> SendMSG(std::string msg, SOCKET* rcp);

private:
    void ProcessMessage(const std::string& msg);
};