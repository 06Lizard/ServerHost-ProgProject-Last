#pragma once
#include "SocketManager.h"
#include "ClientManager.h"
#include "ClientHandler.h"
#include "LoginClient.h"
#include "ThreadPool.h"
#include <iostream>
//#include <vector>
#include <atomic>
#include <unordered_map>
#include "Colors.h"
#include "TMPChat.h"

class Host : public Colors
{
private:
    ///
    SocketManager socketManager;
    ClientManager clientManager;
    ThreadPool threadPool;
    std::atomic<bool> running;
    //std::unordered_map<SOCKET, std::unique_ptr<LoginClient>> loginClients; // Store pointers to manage the lifecycle
    //std::vector<SOCKET> acseptedCLientSockets;

public:
    /// <summary>
    /// Empty constructor, starts the program itself
    /// </summary>
    Host();

    /// <summary>
    /// deconstructor that closses the program properly
    /// </summary>
    ~Host();

private:
    /// <summary>
    /// the function that handles all the clients
    /// </summary>
    void HandleClients();

    /// <summary>
    /// the function that handles induvidual clients that are already logged in
    /// </summary>
    /// <param name="clientIdx"></param>
    void HandleClient(int clientIdx);

    /// <summary>
    /// function that handles all clients that aree not logged in they
    /// </summary>
    void AddClient();


    /// <summary>
    /// TMP selution to talk from just server to clinet instead
    /// </summary>
    void TstStCMSG();
};