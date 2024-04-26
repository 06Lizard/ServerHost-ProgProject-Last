#pragma once
#include <iostream>
#include <winsock2.h>

class Host {
private:
    // General
    const short PORT;

    // Host
    WSADATA wsaData;
    SOCKET serverSocket;
    sockaddr_in serverAddr;

    // Receive Message
    sockaddr_in clientAddr;
    SOCKET clientSocket;

public:
    Host() : PORT(12345) {
        if (StartWSA()) {
            std::cerr << "Failed to initialize Winsock." << std::endl;
            return;
        }

        if (CreateSocket()) {
            std::cerr << "Failed to create socket and bind." << std::endl;
            return;
        }

        if (Listen(1)) {
            std::cerr << "Failed to listen for incoming connections." << std::endl;
            return;
        }

        if (ConnectClient()) {
            std::cerr << "Failed to accept client connection." << std::endl;
            return;
        }

        if (ReceiveMessage()) {
            std::cerr << "Failed to received message." << std::endl;
        }
    }

    ~Host() {
        CloseConnection();
    }

private:
    int StartWSA()
    {
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        {
            std::cerr << "WSAStartup failed: " << WSAGetLastError() << std::endl;
            return 1;
        }
        return 0;
    }

    int CreateSocket()
    {
        serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (serverSocket == INVALID_SOCKET)
        {
            std::cerr << "Error creating socket: " << WSAGetLastError() << std::endl;
            WSACleanup();
            return 1;
        }

        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(PORT);
        serverAddr.sin_addr.s_addr = INADDR_ANY;

        if (bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR)
        {
            std::cerr << "Error binding socket: " << WSAGetLastError() << std::endl;
            closesocket(serverSocket);
            WSACleanup();
            return 1;
        }
        return 0;
    }

    int Listen(int clientLim)
    {
        if (listen(serverSocket, clientLim) == SOCKET_ERROR)
        {
            std::cerr << "Error listening on socket: " << WSAGetLastError() << std::endl;
            CloseConnection();
            return 1;
        }

        std::cout << "Server listening for incoming connections..." << std::endl;
        return 0;
    }

    int ConnectClient()
    {
        int clientAddrSize = sizeof(clientAddr);
        clientSocket = accept(serverSocket, reinterpret_cast<sockaddr*>(&clientAddr), &clientAddrSize);
        if (clientSocket == INVALID_SOCKET)
        {
            std::cerr << "Error accepting connection: " << WSAGetLastError() << std::endl;
            CloseConnection();
            return 1;
        }
        else {
            std::cout << "Client connected" << std::endl;
            return 0;
        }
    }

    int ReceiveMessage()
    {
        char buffer[1024];
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived == SOCKET_ERROR)
        {
            std::cerr << "Error receiving data: " << WSAGetLastError() << std::endl;
            CloseConnection();
            return 1;
        }
        else {
            buffer[bytesReceived] = '\0';
            std::cout << "Received message:\t" << buffer << std::endl;
            return 0;
        }
    }

    void CloseConnection()
    {
        closesocket(clientSocket);
        closesocket(serverSocket);
        WSACleanup();
    }
};