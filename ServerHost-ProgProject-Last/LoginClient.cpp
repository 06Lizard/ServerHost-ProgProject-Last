#include "LoginClient.h"
#include <cctype> // For std::isalpha
#include <algorithm> // For std::all_of>

LoginClient::LoginClient(SOCKET clientSocket, ClientManager* clientManager)
    : handler(clientSocket, clientManager), clientManager(clientManager) {
    handler.SendMSG("Type 'login' to login to server, type 'signup' to sign up to server", clientSocket);
    std::cout << "awaiting login/signup" << std::endl;
    while (true) {
        std::string msg = ReceiveLoginMSG(clientSocket);
        if (msg == "login") {
            Login(clientSocket);
            break;
        }
        else if (msg == "signup") {
            SignUp(clientSocket);
            break;
        }
        else {
            handler.SendMSG("Invalid command. Please type 'login' or 'signup'.", clientSocket);
            continue;
        }
    }
}

void LoginClient::Login(SOCKET clientSocket) {
    std::string username = ReceiveLoginMSG(clientSocket);
    while (!std::all_of(username.begin(), username.end(), ::isalpha) || username == "login" || username == "signup" || username == "server") {
        handler.SendMSG("Invalid username. Please enter a valid username:", clientSocket);
        username = ReceiveLoginMSG(clientSocket);
    }

    for (int attempts = 0; attempts < 3; ++attempts) {
        std::string password = ReceiveLoginMSG(clientSocket);
        if (clientManager->ValidatePassword(username, password)) {
            handler.SendMSG("Login successful.", clientSocket);
            //clientManager->SetClientOnline(username, clientSocket); // haven't implemented online / offline function yet and might never
            return;
        }
        else {
            handler.SendMSG("Invalid password. Try again:", clientSocket);
        }
    }

    handler.SendMSG("Too many failed attempts. Disconnecting...", clientSocket);
    closesocket(clientSocket);
}

void LoginClient::SignUp(SOCKET clientSocket) {
    std::string username = ReceiveLoginMSG(clientSocket);
    while (!std::all_of(username.begin(), username.end(), ::isalpha) || username == "login" || username == "signup" || username == "server") {
        handler.SendMSG("Invalid username. Please enter a valid username:", clientSocket);
        username = ReceiveLoginMSG(clientSocket);
    }

    std::string password;
    std::string verifyPassword;
    do {
        handler.SendMSG("Enter new password:", clientSocket);
        password = ReceiveLoginMSG(clientSocket);
        handler.SendMSG("Verify password:", clientSocket);
        verifyPassword = ReceiveLoginMSG(clientSocket);
        if (password != verifyPassword) {
            handler.SendMSG("Passwords do not match.", clientSocket);
        }
    } while (password != verifyPassword);
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