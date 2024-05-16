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
    /// <summary>
    /// 
    /// </summary>
    /// <param name="socket"></param>
    ClientHandler(SOCKET socket);

    /// <summary>
    /// 
    /// </summary>
    ~ClientHandler();

    /// <summary>
    /// 
    /// </summary>
    void ReceiveMSG();

    /// <summary>
    /// Sends the message 'msg' to client 'rcp'
    /// </summary>
    /// <param name="msg"></param>
    /// <param name=""></param>
    /// <returns></returns>
    std::future<int> SendMSG(std::string msg, SOCKET rcp);

    /// <summary>
    /// 
    /// </summary>
    /// <param name="msg"></param>
    /// <returns></returns>
    std::future<int> LogMSG(std::string msg);
};