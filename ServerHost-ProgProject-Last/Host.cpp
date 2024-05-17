#include "Host.h"

Host::Host() : running(true), threadPool(4) {
    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize Winsock." << std::endl;
        return;
    }
    else {
        std::cerr << "Successfully initialized Winsock." << std::endl;
    }

    // Open the socket
    if (!socketManager.Start(12345)) { // hardcoded port for now
        std::cerr << "Failed to start server." << std::endl;
        WSACleanup();
        return;
    }
    else {
        std::cerr << "Successfully started server." << std::endl;
    }

    // Handle clients in the main thread
    HandleClients();
}

Host::~Host() {
    running = false;
    socketManager.Close();
    WSACleanup();
    std::cerr << "Closed server." << std::endl;
}

void Host::HandleClients() {
    while (running) {
        std::vector<SOCKET> clientSockets = socketManager.CheckEvents();

        for (SOCKET clientSocket : clientSockets) {
            if (clientSocket == socketManager.GetListeningSocket()) {
                SOCKET newClientSocket = socketManager.AcceptClient();
                std::cout << "newClientSocket made" << std::endl;
                if (newClientSocket != INVALID_SOCKET) {
                    LoginClient loginClient(newClientSocket, &clientManager);
                }
                else {
                    std::cerr << "Error loggin in" << std::endl;
                    closesocket(newClientSocket);
                }
            }
            else {
                threadPool.enqueue([this, clientSocket]() { HandleClient(clientSocket); });
            }
        }
    }
}

void Host::HandleClient(SOCKET clientSocket) {
    ClientHandler handler(clientSocket, &clientManager);
    handler.ReceiveMSG();
}

                


//threadPool.enqueue([this, clientSocket]() { AddClient(); });
void Host::AddClient() { //curently not in use
    SOCKET newClientSocket = socketManager.AcceptClient();
    if (newClientSocket != INVALID_SOCKET) {
        LoginClient loginClient(newClientSocket, &clientManager);
    }
}