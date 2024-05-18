#include "LoginClient.h"
#include <cctype> // For std::isalpha
#include <algorithm> // For std::all_of>

LoginClient::LoginClient(SOCKET clientSocket, ClientManager* clientManager)
    : handler(clientSocket, clientManager), clientManager(clientManager) {
    handler.SendMSG("Type 'login' to login to server, type 'signup' to sign up to server", clientSocket);
    std::cout << "awaiting login/signup" << std::endl;
    bool loop = true;
    while (loop) {
        //std::cout << "loop" << std::endl;

        std::string msg = ReceiveLoginMSG(clientSocket);
        if (msg == "login") {
            Login(clientSocket);
            break; //exits while
        }
        else if (msg == "signup") {
            SignUp(clientSocket);
            break; //exits while
        }
        else {
            for (int i = 0; i < 3; i++) { //lops 3 times
                std::future<int> futureResult = handler.SendMSG("Invalid command. Please type 'login' or 'signup'.", clientSocket);                
                //std::cout << "hi" << std::endl;
                if (futureResult.get() == 0) {
                    break; //exits for
                }
                else if (i > 1) { //third atempt or more
                    closesocket(clientSocket);
                    loop = false; //ends while to exit it
                }
            }
        }
    }
}

void LoginClient::Login(SOCKET clientSocket) {
    handler.SendMSG("Login initiated, enter username.", clientSocket);
    std::string username = ReceiveLoginMSG(clientSocket);
    while (!std::all_of(username.begin(), username.end(), ::isalpha) || username == "login" || username == "signup" || username == "server") { // add a check for existing usernames too
        handler.SendMSG("Invalid username. Please enter a valid username:", clientSocket);
        username = ReceiveLoginMSG(clientSocket);
    }

    handler.SendMSG("User found, enter pasword.", clientSocket);
    for (int attempts = 0; attempts < 3; ++attempts) {
        std::string password = ReceiveLoginMSG(clientSocket);
        if (clientManager->ValidatePassword(username, password)) {
            handler.SendMSG("Login successful.", clientSocket);
            //clientManager->SetClientOnline(username, clientSocket); // haven't implemented online / offline function yet and might never
            return;
        }
        else if (attempts < 2) {
            handler.SendMSG("Invalid password. Try again:", clientSocket);
        }
        else {
            handler.SendMSG("Too many failed attempts. Disconnecting...", clientSocket);
            closesocket(clientSocket);
        }
    }
}

void LoginClient::SignUp(SOCKET clientSocket) {
    handler.SendMSG("SignUp initiated, enter username.", clientSocket);
    std::string username = ReceiveLoginMSG(clientSocket);
    while (!std::all_of(username.begin(), username.end(), ::isalpha) || username == "login" || username == "signup" || username == "server") {
        handler.SendMSG("Invalid username. Please enter a valid username:", clientSocket);
        username = ReceiveLoginMSG(clientSocket);
    }

    std::string password = "";
    std::string verifyPassword = "";
    do {
        handler.SendMSG("Enter new password:", clientSocket);
        password = ReceiveLoginMSG(clientSocket);
        handler.SendMSG("Verify password:", clientSocket);
        verifyPassword = ReceiveLoginMSG(clientSocket);
        if (password != verifyPassword || password == "") {
            handler.SendMSG("Passwords does not match.", clientSocket);
        }
    } while (password != verifyPassword || password == "");
    clientManager->AddClient(username, clientSocket, password);
    handler.SendMSG("Signup successful.", clientSocket);
}

std::string LoginClient::ReceiveLoginMSG(SOCKET clientSocket) {
    char buffer[1024];
    int result = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (result > 0) {
        return std::string(buffer, result);
    }
    else {
        return "";
    }
}