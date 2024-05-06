#include "SocketManager.h"

SocketManager::SocketManager() : serverSocket(INVALID_SOCKET) {
}

SocketManager::~SocketManager() {
    Close();
}

bool SocketManager::Start(int port) {
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        return false;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR) {
        return false;
    }

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        return false;
    }

    return true;
}

SOCKET SocketManager::AcceptClient() {
    sockaddr_in clientAddr;
    int clientAddrSize = sizeof(clientAddr);
    return accept(serverSocket, reinterpret_cast<sockaddr*>(&clientAddr), &clientAddrSize);
}

void SocketManager::Close() {
    if (serverSocket != INVALID_SOCKET) {
        closesocket(serverSocket);
    }
}