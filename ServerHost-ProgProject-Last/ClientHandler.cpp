#include "ClientHandler.h"

ClientHandler::ClientHandler(SOCKET socket, ClientManager* manager)
    : clientSocket(socket), clientManager(manager), running(true) {}

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
            std::string msg(buffer);
            ProcessMessage(msg);
        }
    }
}

void ClientHandler::ProcessMessage(const std::string& msg) {
    // Expected format: "<recipient_id>:<message>"
    size_t delimiterPos = msg.find(':');
    if (delimiterPos != std::string::npos) {
        std::string recipientId = msg.substr(0, delimiterPos);
        std::string actualMsg = msg.substr(delimiterPos + 1);

        if (clientManager->ClientExists(recipientId)) {
            SOCKET recipientSocket = clientManager->GetClientSocket(recipientId);
            SendMSG(actualMsg, recipientSocket);
        }
        else {
            std::cerr << "Recipient not found: " << recipientId << std::endl;
        }
    }
    else {
        std::cerr << "Invalid message format received: " << msg << std::endl;
    }
}

std::future<int> ClientHandler::SendMSG(std::string msg, SOCKET rcp) {
    return std::async(std::launch::async, [this, msg, rcp]() -> int {
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
        save = msg;
        // Handle the message
        SendMSG("handled message", clientSocket);
        return 0;
        });
}