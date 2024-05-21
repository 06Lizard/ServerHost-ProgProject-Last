#pragma once
#include <string>
#include "ClientHandler.h"

class LoginClient
{
public:
    /// <summary>
    /// Constructor that begins the loggin proces of new clients
    /// </summary>
    /// <param name="idx"></param>
    /// <param name="clientManager"></param>
    LoginClient(int idx, ClientManager* clientManager);
private:
    /// <summary>
    /// Logs in the client using it's idx in the clientS map
    /// </summary>
    /// <param name="idx"></param>
    void Login(int idx);

    /// <summary>
    /// signs up new clients using the clientS map
    /// </summary>
    /// <param name="idx"></param>
    void SignUp(int idx);   

    /// <summary>
    /// Recives messages from clients specifically during th einlog proces, wanted to make this a overide on ReciveMSG but is to diferent to overide
    /// </summary>
    /// <param name="clientSocket"></param>
    /// <returns></returns>
    std::string ReceiveLoginMSG(SOCKET* clientSocket);

    /// <summary>
    /// Tries to echo back the messages to the client and if it fails it returns fallse and closes the client socket
    /// </summary>
    /// <param name="msg"></param>
    /// <param name="clientSocket"></param>
    /// <returns></returns>
    bool EchoBack(std::string msg, SOCKET* clientSocket);

    ClientHandler handler;
    ClientManager* clientManager;
    bool running;
};
