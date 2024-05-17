#pragma once
#include "SocketManager.h"
#include "ClientManager.h"
#include "ClientHandler.h"
#include "LoginClient.h"
#include "ThreadPool.h"
#include <iostream>
#include <vector>
#include <atomic>

class Host {
private:
    SocketManager socketManager;
    ClientManager clientManager;
    ThreadPool threadPool;
    std::atomic<bool> running;

public:
    Host();
    ~Host();

private:
    void HandleClients();
    void HandleClient(SOCKET clientSocket);
    void AddClient();
};