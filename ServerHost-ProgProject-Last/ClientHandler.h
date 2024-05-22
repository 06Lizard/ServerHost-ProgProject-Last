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
    int idx; // idx of client
    std::string save; // save "file" (temporary store location as msg loggin was never implemented)
    std::string clientId; // the username of the server
    ClientManager* clientManager; // refrence to host's client manager
    std::atomic<bool> running; // bool to keep the client handle running

public:
    /// <summary>
    /// Constructor: takes in the inx for client it's handling and a refrence to the servers client handler
    /// </summary>
    /// <param name="idx"></param>
    /// <param name="manager"></param>
    ClientHandler(int idx, ClientManager* manager);

    /// <summary>
    /// deconstructor so that we can be sure the clientHandler is closed properly
    /// </summary>
    ~ClientHandler();

    /// <summary>
    /// the function that recives messages and later sends them off to be proccesed
    /// </summary>
    std::future<void> ReceiveMSG();

    /// <summary>
    /// the function that sends messages to clients
    /// </summary>
    /// <param name="msg"></param>
    /// <param name="rcp"></param>
    /// <returns></returns>
    std::future<int> SendMSG(std::string msg, SOCKET* rcp);

private:
    /// <summary>
    /// the function that proceses the messages it recives and makes the server do the coresponding actions such as forwarding the message to the corect client
    /// </summary>
    /// <param name="msg"></param>
    void ProcessMessage(const std::string& msg);
};