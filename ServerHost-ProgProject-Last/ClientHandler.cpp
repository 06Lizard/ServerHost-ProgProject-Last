#include "ClientHandler.h"

ClientHandler::ClientHandler(SOCKET socket) : clientSocket(socket), running(true) {
}

ClientHandler::~ClientHandler() {
    closesocket(clientSocket);
}

void ClientHandler::ReceiveMSG() {
    char buffer[1024];
    int bytesReceived;

    while (running) {
        bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            std::cout << "Received message: " << buffer << std::endl;
            LogMSG(buffer);
        }
    }
}

std::future<int> ClientHandler::SendMSG(std::string msg, SOCKET rcp) {
    return std::async(std::launch::async, [this, msg, rcp]() -> int { // Add rcp to the capture list
        if (send(rcp, msg.c_str(), strlen(msg.c_str()), 0) == SOCKET_ERROR) {
            std::cerr << "Error sending data: " << WSAGetLastError() << " with the message '" << msg << "'" << std::endl;
            return WSAGetLastError();
        }
        else {
            std::cout << "Successfully sent message: " << msg << std::endl;
            return 0;
        }
    });
}

std::future<int> ClientHandler::LogMSG(std::string msg) {
    return std::async(std::launch::async, [this, msg]() -> int {
        // handle where to save message at
        save = msg;

        // handle message in some way, like where to send        
        SendMSG("handled message", clientSocket);

        return 0;
    });
}