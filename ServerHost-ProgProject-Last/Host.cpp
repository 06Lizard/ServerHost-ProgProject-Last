#include "Host.h"

Host::Host() : running(true), threadPool(4) {
    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize Winsock." << std::endl;
        return;
    }
    else {
        std::cout << "Successfully initialized Winsock." << std::endl;
    }

    // Open the socket
    if (!socketManager.Start(12345)) { // hardcoded port for now
        std::cerr << "Failed to start server." << std::endl;
        WSACleanup();
        return;
    }
    else {
        std::cout << "Successfully started server." << std::endl;
    }

    // Handle clients in the main thread
    HandleClients();
}

Host::~Host() {
    running = false;
    socketManager.Close();
    WSACleanup();
    std::cout << "Closed server." << std::endl;
}

void Host::HandleClients() {
    while (running) {
        std::vector<SOCKET> clientSockets = socketManager.CheckEvents(); // list of all sockets with events

        for (SOCKET clientSocket : clientSockets) {
            if (clientSocket == socketManager.GetListeningSocket()) {
                //threadPool.enqueue([this]() { AddClient(); });
                AddClient();
            }
            else {
                threadPool.enqueue([this, clientSocket]() { HandleClient(clientSocket); });
            }
        }
    }
}

void Host::HandleClient(SOCKET clientSocket) {
    std::cout << "H->HC" << std::endl;

    ClientHandler handler(clientSocket, &clientManager);
    handler.ReceiveMSG();
}

void Host::AddClient() {
    SOCKET newClientSocket = socketManager.AcceptClient();
    if (newClientSocket != INVALID_SOCKET) {
        threadPool.enqueue([this, newClientSocket]() {
            LoginClient loginClient(newClientSocket, &clientManager);
            // Post login, client socket is managed by ClientManager
            });
    }
    else {
        std::cerr << "Error logging in" << std::endl;
        closesocket(newClientSocket);
    }
}