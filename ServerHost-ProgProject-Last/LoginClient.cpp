#include "LoginClient.h"
#include <cctype>
#include <algorithm>

LoginClient::LoginClient(SOCKET clientSocket, ClientManager* clientManager)
    : handler(clientSocket, clientManager), clientManager(clientManager) {
    running = EchoBack("Type 'login' to login to server, type 'signup' to sign up to server", clientSocket);

    while (running) {
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
            running = EchoBack("Invalid command. Please type 'login' or 'signup'.", clientSocket);
        }
    }
}

void LoginClient::Login(SOCKET clientSocket) {
    running = EchoBack("Login initiated, enter username.", clientSocket);
    if (!running) { return; }
    std::string username;
    while (running) {
        username = clientManager->str_tolower(ReceiveLoginMSG(clientSocket));
        if (!std::all_of(username.begin(), username.end(), ::isalpha) || username == "login" || username == "signup" || username == "server") {
            running = EchoBack("Invalid username. Please enter a valid username:", clientSocket);
        }
        else if (!clientManager->ClientExists(username)) {
            running = EchoBack("User not found, re-enter username.", clientSocket);
        }
        else {
            break;
        }
    }

    if (!running) { return; }

    running = EchoBack("User found, enter password.", clientSocket);

    if (!running) { return; }

    for (int attempts = 0; attempts < 3; ++attempts) {
        std::string password = ReceiveLoginMSG(clientSocket);
        if (clientManager->ValidatePassword(username, password)) {
            EchoBack("Login successful.", clientSocket);
            clientManager->AddClient(username, clientSocket, password); // Add client to ClientManager
            return;
        }
        else if (attempts < 2) {
            running = EchoBack("Invalid password. Try again:", clientSocket);
            if (!running) { return; }
        }
        else {
            handler.SendMSG("Too many failed attempts. Disconnecting...", clientSocket);
            closesocket(clientSocket);
        }
    }
}

void LoginClient::SignUp(SOCKET clientSocket) {
    running = EchoBack("SignUp initiated, enter username:", clientSocket);
    if (!running) { return; }

    std::string username;
    while (running) {
        username = clientManager->str_tolower(ReceiveLoginMSG(clientSocket));
        if (!std::all_of(username.begin(), username.end(), ::isalpha) || username == "login" || username == "signup" || username == "server") {
            running = EchoBack("Invalid username. Please enter a valid username:", clientSocket);
        }
        else if (clientManager->ClientExists(username)) {
            running = EchoBack("User already exists. Please enter another username:", clientSocket);
        }
        else {
            break;
        }
    }

    if (!running) { return; }

    std::string password = "";
    std::string verifyPassword = "";
    do {
        running = EchoBack("Enter new password:", clientSocket);
        password = ReceiveLoginMSG(clientSocket);
        running = EchoBack("Verify password:", clientSocket);
        verifyPassword = ReceiveLoginMSG(clientSocket);
        if (password != verifyPassword || password.empty()) {
            running = EchoBack("Passwords do not match.", clientSocket);
        }
    } while (running && (password != verifyPassword || password.empty()));

    clientManager->AddClient(username, clientSocket, password); // Add client to ClientManager
    EchoBack("Signup successful.", clientSocket);
}

std::string LoginClient::ReceiveLoginMSG(SOCKET clientSocket) {
    char buffer[1024];
    int result = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (result > 0) {
        return std::string(buffer, result);
    }
    else {
        running = false;
        return "";
    }
}

bool LoginClient::EchoBack(const std::string msg, SOCKET clientSocket) {
    for (int i = 0; i < 3; ++i) {
        std::future<int> futureResult = handler.SendMSG(msg, clientSocket);
        if (futureResult.get() == 0) {
            return true;
        }
        else if (i >= 2) {
            closesocket(clientSocket);
            return false;
        }
    }
    return true;
}
