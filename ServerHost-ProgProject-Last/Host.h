#pragma once
#include "SocketManager.h"
#include <iostream>
#include <thread>
#include <vector>
#include <atomic>

class Host {
private:
    SocketManager socketManager;
    std::atomic<bool> running;

public:
    Host();
    ~Host();

private:
    void HandleClients();
    void HandleClient(SOCKET clientSocket);
};