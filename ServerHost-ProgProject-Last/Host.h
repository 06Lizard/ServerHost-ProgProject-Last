#pragma once
#include "SocketManager.h"
#include "ThreadPool.h"
#include <iostream>
#include <vector>
#include <atomic>

class Host {
private:
    SocketManager socketManager;
    ThreadPool threadPool;
    std::atomic<bool> running;

public:
    Host();
    ~Host();

private:
    void HandleClients();
    void HandleClient(SOCKET clientSocket);
};