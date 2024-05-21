#include "SocketManager.h"

SocketManager::SocketManager() : serverSocket(INVALID_SOCKET) 
{
}

SocketManager::~SocketManager()
{
    Close();
}

bool SocketManager::Start(int port) 
{
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) 
    {
        PrintError("Error, could no create socket.");
        Close();
        return false;
    }
    else 
    {
        PrintSuccessful("Server socket created.");
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR)
    {
        PrintError("Bind failed.");
        Close();
        return false;
    }

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        PrintError("Listen failed.");
        Close();
        return false;
    }
    else
    {
        PrintWarning("Server listening on socket");
    }

    return true;
}

// Cannot print messages with PrintError() or PrintSuccessful() in this function because it is constant
SOCKET SocketManager::AcceptClient() const 
{
    sockaddr_in clientAddr;
    int clientAddrSize = sizeof(clientAddr);
    std::cout << "Waiting for client connection." << std::endl;
    SOCKET clientSocket = accept(serverSocket, reinterpret_cast<sockaddr*>(&clientAddr), &clientAddrSize);
    if (clientSocket == INVALID_SOCKET) 
    {
        std::cerr << "Error accepting client connection." << std::endl;
    }
    else 
    {
        std::cout << "Successfully accepted client connection." << std::endl;
    }

    return clientSocket;
}

void SocketManager::Close()
{
    if (serverSocket != INVALID_SOCKET)
    {
        closesocket(serverSocket);
    }
}

SOCKET SocketManager::GetListeningSocket() const 
{
    return serverSocket;
}

// Same issue as AcceptClient() with it being constant
std::vector<SOCKET> SocketManager::CheckEvents() const
{
    std::vector<SOCKET> clientSockets;
    fd_set readSet;
    FD_ZERO(&readSet);
    FD_SET(serverSocket, &readSet);
    timeval timeout = { 0, 0 };
    if (select(0, &readSet, nullptr, nullptr, &timeout) == SOCKET_ERROR)
    {
        std::cerr << "Error in select." << std::endl;
        return clientSockets;
    }
    if (FD_ISSET(serverSocket, &readSet))
    {
        clientSockets.push_back(serverSocket);
    }
    return clientSockets;
}