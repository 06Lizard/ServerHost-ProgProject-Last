#pragma once
#include <string>
#include "ClientHandler.h"

class LoginClient {
public:
    LoginClient(SOCKET clientSocket, ClientManager* clientManager);
private:
    void Login(SOCKET clientSocket);
    void SignUp(SOCKET clientSocket);
    std::string ReceiveLoginMSG(SOCKET clientSocket);
    ClientHandler handler;
    ClientManager* clientManager;
};
