#include "LoginClient.h"
#include <cctype> // For std::isalpha
#include <algorithm> // For std::all_of>

LoginClient::LoginClient(SOCKET clientSocket, ClientManager* clientManager)
    : handler(clientSocket, clientManager), clientManager(clientManager) {
    running = EchoBack("Type 'login' to login to server, type 'signup' to sign up to server", clientSocket);
    //std::cout << "awaiting login/signup" << std::endl;
    while (running) {
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
            running = EchoBack("Invalid command. Please type 'login' or 'signup'.", clientSocket);
        }
    }
}

void LoginClient::Login(SOCKET clientSocket) {
    running = EchoBack("Login initiated, enter username.", clientSocket);
    if (!running) { return; } // if echo failed exit login
    std::string username;
    while (running)
    {
        username = clientManager->str_tolower(ReceiveLoginMSG(clientSocket));
        if (!std::all_of(username.begin(), username.end(), ::isalpha) || username == "login" || username == "signup" || username == "server") { // add a check for existing usernames too
            running = EchoBack("Invalid username. Please enter a valid username:", clientSocket);
        }
        else if (!clientManager->ClientExists(username)) {
            running = EchoBack("User not found, re-enter username.", clientSocket);
        } else break; //if valid user
    }

    if (!running) { return;  } // exit login if not running = true

    running = EchoBack("User found, enter pasword.", clientSocket);

    if (!running) { return; } // exit login if not running = true

    for (int attempts = 0; attempts < 3; ++attempts) {
        std::string password = ReceiveLoginMSG(clientSocket);
        if (clientManager->ValidatePassword(username, password)) {
            EchoBack("Login successful.", clientSocket);
            //clientManager->SetClientOnline(username, clientSocket); // haven't implemented online / offline function yet and might never
            return;
        }
        else if (attempts < 2) {
            running = EchoBack("Invalid password. Try again:", clientSocket);
            if (!running) { return; } // exit login if not running = true
        }
        else {
            handler.SendMSG("Too many failed attempts. Disconnecting...", clientSocket); // this is still send msg becuse it's gona disconect client anyways
            closesocket(clientSocket);
        }
    }
}

void LoginClient::SignUp(SOCKET clientSocket) {
    running = EchoBack("SignUp initiated, enter username:", clientSocket);
    if (!running) { return; } // exit signup if not running = true

    std::string username;
    while (running)
    {
        username = clientManager->str_tolower(ReceiveLoginMSG(clientSocket));
        if (!std::all_of(username.begin(), username.end(), ::isalpha) || username == "login" || username == "signup" || username == "server") {
            running = EchoBack("Invalid username. Please enter a valid username:", clientSocket);
        }
        else if (clientManager->ClientExists(username)) {
            running = EchoBack("User already exists- Please enter another username:", clientSocket);
            username = clientManager->str_tolower(ReceiveLoginMSG(clientSocket));
        } else break;
    }

    if (!running) { return; } // exit signup if not running = true

    std::string password = "";
    std::string verifyPassword = "";
    do {
        running = EchoBack("Enter new password:", clientSocket);
        password = ReceiveLoginMSG(clientSocket);
        running = EchoBack("Verify password:", clientSocket);
        verifyPassword = ReceiveLoginMSG(clientSocket);
        if (password != verifyPassword || password == "") {
            running = EchoBack("Passwords does not match.", clientSocket);
        }
    } while (running && password != verifyPassword || running && password == "");
    clientManager->AddClient(username, clientSocket, password);
    EchoBack("Signup successful.", clientSocket);
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

bool LoginClient::EchoBack(std::string msg, SOCKET clientSocket) {
    for (int i = 0; i < 3; i++) { //lops 3 times
        std::future<int> futureResult = handler.SendMSG(msg, clientSocket);
        if (futureResult.get() == 0) {
            break; //exits for
        }
        else if (i > 1) { //third atempt or more
            closesocket(clientSocket);
            return false;
        }
    }
    return true;
}