#pragma once
#include <string>
#include "ClientHandler.h"

class LoginClient {
public:
    LoginClient(int idx, ClientManager* clientManager);
private:
    void Login(int idx);
    void SignUp(int idx);
    std::string ReceiveLoginMSG(SOCKET* clientSocket);
    bool EchoBack(std::string msg, SOCKET* clientSocket);
    ClientHandler handler;
    ClientManager* clientManager;
    bool running;
};
