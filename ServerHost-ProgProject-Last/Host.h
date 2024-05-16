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
    /// <summary>
    /// Constructor that inizialises WSADATA
    /// Then it starts the SocketManager
    /// Then goes to HandleClients
    /// </summary>
    Host();

    /// <summary>
    /// Deconstructs the entire server (add gracefull Shutdown?)
    /// </summary>
    ~Host();

private:
    /// <summary>
    /// Handles all the clients, acsepting them and/or sending them to the treadpool executing the function HandleClients
    /// </summary>
    void HandleClients();

    /// <summary>
    /// Handles the inputed client via the threadpool
    /// </summary>
    /// <param name="clientSocket"></param>
    void HandleClient(SOCKET clientSocket);
};