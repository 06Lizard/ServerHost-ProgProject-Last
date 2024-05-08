#include "SocketManager.h"

SocketManager::SocketManager() : serverSocket(INVALID_SOCKET) {
}

SocketManager::~SocketManager() {
    Close();
}

bool SocketManager::Start(int port) {
    // create the server socket
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "ERROR: Could not create socket." << std::endl;
        Close();
        return false;
    }
    else 
        std::cerr << "serverSocket created" << std::endl;

    //set server addr
    serverAddr.sin_family = AF_INET; // what type is the server adr
    serverAddr.sin_port = htons(port); // what port is the server on
    serverAddr.sin_addr.s_addr = INADDR_ANY; // what IP's can connect

    // bind serverSocket and serverAddr
    if (bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed" << std::endl;
        Close();
        return false;
    }

    // start listening on socket
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed" << std::endl;
        Close();
        return false;
    }
    else
        std::cerr << "Server listenng on socket" << std::endl;

    return true;
}


SOCKET SocketManager::AcceptClient() {
    sockaddr_in clientAddr;
    int clientAddrSize = sizeof(clientAddr);
    std::cerr << "Waiting for client connection." << std::endl;
    return accept(serverSocket, reinterpret_cast<sockaddr*>(&clientAddr), &clientAddrSize);
}

void SocketManager::Close() {
    if (serverSocket != INVALID_SOCKET) {
        closesocket(serverSocket);
    }
}