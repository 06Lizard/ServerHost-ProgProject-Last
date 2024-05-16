#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <vector>

class SocketManager {
private:
    SOCKET serverSocket;
    sockaddr_in serverAddr;

public:
    /// <summary>
    /// 
    /// </summary>
    SocketManager();

    /// <summary>
    /// 
    /// </summary>
    ~SocketManager();

    /// <summary>
    /// 
    /// </summary>
    /// <param name="port"></param>
    /// <returns></returns>
    bool Start(int port);

    /// <summary>
    /// 
    /// </summary>
    /// <returns></returns>
    SOCKET AcceptClient() const;

    /// <summary>
    /// 
    /// </summary>
    void Close();

    /// <summary>
    /// 
    /// </summary>
    /// <returns></returns>
    SOCKET GetListeningSocket() const;

    /// <summary>
    /// 
    /// </summary>
    /// <returns></returns>
    std::vector<SOCKET> CheckEvents() const;
};