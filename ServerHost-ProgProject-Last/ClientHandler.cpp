#include "ClientHandler.h"
#include <iostream>

ClientHandler::ClientHandler(SOCKET socket) : clientSocket(socket) {
}

ClientHandler::~ClientHandler() {
    closesocket(clientSocket);
}

void ClientHandler::ReceiveMessages() 
{
    char buffer[1024];
    int bytesReceived;

    while ((bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
        buffer[bytesReceived] = '\0';
        std::cout << "Received message: " << buffer << std::endl;
    }

    if (bytesReceived == 0) {
        std::cout << "Client disconnected." << std::endl;
    }
    else {
        std::cerr << "Error receiving data." << std::endl;
    }
}

void ClientHandler::SendMessages() 
{

}