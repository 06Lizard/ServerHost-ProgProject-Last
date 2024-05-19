#pragma once
#include "SocketManager.h"
#include "ClientManager.h"
#include "ClientHandler.h"
#include "LoginClient.h"
#include "ThreadPool.h"
#include <iostream>
#include <vector>
#include <atomic>
#include <unordered_map>

class Host {
private:
    SocketManager socketManager;
    ClientManager clientManager;
    ThreadPool threadPool;
    std::atomic<bool> running;
    std::unordered_map<SOCKET, std::unique_ptr<LoginClient>> loginClients; // Store pointers to manage the lifecycle
    std::vector<SOCKET> acseptedCLientSockets;

public:
    Host();
    ~Host();

private:
    void HandleClients();
    void HandleClient(int clientIdx);
    void AddClient();
};