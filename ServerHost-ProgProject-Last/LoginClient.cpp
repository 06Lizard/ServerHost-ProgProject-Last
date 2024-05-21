#include "LoginClient.h"
#include <cctype>
#include <algorithm>

LoginClient::LoginClient(int idx, ClientManager* clientManager)
    : handler(idx, clientManager), clientManager(clientManager) 
{
    running = EchoBack("Type 'login' to login to server, type 'signup' to sign up to server", clientManager->GetClientSocket(idx));

    while (running) {
        std::string msg = ReceiveLoginMSG(clientManager->GetClientSocket(idx));
        if (msg == "login") {
            Login(idx);
            break;
        }
        else if (msg == "signup") {
            SignUp(idx);
            break;
        }
        else {
            running = EchoBack("Invalid command. Please type 'login' or 'signup'.", clientManager->GetClientSocket(idx));
        }
    }
    EchoBack("Exiting LoginClientClass.", clientManager->GetClientSocket(idx));
}

void LoginClient::Login(int idx) 
{
    running = EchoBack("Login initiated, enter username.", clientManager->GetClientSocket(idx));
    if (!running) { return; }
    
    std::string username;
    while (running) 
    {
        username = clientManager->str_tolower(ReceiveLoginMSG(clientManager->GetClientSocket(idx)));
        if (!std::all_of(username.begin(), username.end(), ::isalpha) || username == "login" || username == "signup" || username == "server") 
        {
            running = EchoBack("Invalid username. Please enter a valid username.", clientManager->GetClientSocket(idx));
        }
        else if (!clientManager->ClientExists(idx)) {
            running = EchoBack("User not found, re-enter username.", clientManager->GetClientSocket(idx));
        }
        else 
        {
            break;
        }
    }

    if (!running) { return; }

    running = EchoBack("User found, enter password.", clientManager->GetClientSocket(idx));

    if (!running) { return; }

    for (int attempts = 0; attempts < 3; ++attempts) 
    {
        std::string password = ReceiveLoginMSG(clientManager->GetClientSocket(idx));
        if (clientManager->ValidatePassword(username, password)) 
        {
            EchoBack("Login successful.", clientManager->GetClientSocket(idx));
            clientManager->LoginClient(username, password, idx); // Add client to ClientManager
            return;
        }
        else if (attempts < 2) 
        {
            running = EchoBack("Invalid password. Try again.", clientManager->GetClientSocket(idx));
            if (!running) { return; }
        }
        else 
        {
            handler.SendMSG("Too many failed attempts. Disconnecting...", clientManager->GetClientSocket(idx));
            closesocket(*clientManager->GetClientSocket(idx));
        }
    }
}

void LoginClient::SignUp(int idx) 
{
    running = EchoBack("SignUp initiated, enter username.", clientManager->GetClientSocket(idx));
    if (!running) { return; }

    std::string username;
    while (running) 
    {
        username = clientManager->str_tolower(ReceiveLoginMSG(clientManager->GetClientSocket(idx)));
        if (!std::all_of(username.begin(), username.end(), ::isalpha) || username == "login" || username == "signup" || username == "server") 
        {
            running = EchoBack("Invalid username. Please enter a valid username.", clientManager->GetClientSocket(idx));
        }
        else if (clientManager->ID_FromUsr(username) != -1) 
        {
            running = EchoBack("User already exists. Please enter another username.", clientManager->GetClientSocket(idx));
        }
        else 
        {
            break;
        }
    }

    if (!running) { return; }

    std::string password = "";
    std::string verifyPassword = "";
    do 
    {
        running = EchoBack("Enter new password.", clientManager->GetClientSocket(idx));
        password = ReceiveLoginMSG(clientManager->GetClientSocket(idx));
        running = EchoBack("Verify password.", clientManager->GetClientSocket(idx));
        verifyPassword = ReceiveLoginMSG(clientManager->GetClientSocket(idx));
        if (password != verifyPassword || password.empty()) 
        {
            running = EchoBack("Passwords do not match.", clientManager->GetClientSocket(idx));
        }
    } while (running && (password != verifyPassword || password.empty()));

    clientManager->LoginClient(username, password, idx); // Add client to ClientManager
    EchoBack("Signup successful.", clientManager->GetClientSocket(idx));
}

std::string LoginClient::ReceiveLoginMSG(SOCKET* clientSocket) // make this override the ReciveMSG from client manager
{
    char buffer[1024];
    int result = recv(*clientSocket, buffer, sizeof(buffer), 0);
    if (result > 0) 
    {
        return std::string(buffer, result);
    }
    else 
    {
        running = false;
        return "";
    }
}

bool LoginClient::EchoBack(const std::string msg, SOCKET* clientSocket) 
{
    for (int i = 0; i < 3; ++i) {
        std::future<int> futureResult = handler.SendMSG(msg, clientSocket);
        if (futureResult.get() == 0)
        {
            return true;
        }
        else if (i >= 2) 
        {
            std::cerr << "disconecting server" << std::endl;
            closesocket(*clientSocket);
            return false;
        }
    }
    return true;
}
