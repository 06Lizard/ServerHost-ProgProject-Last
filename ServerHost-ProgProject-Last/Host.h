#pragma once
#include <iostream>
#include <winsock2.h>

class Host {
private:
    //Host
    WSADATA wsaData;
    SOCKET serverSocket;
    sockaddr_in serverAddr;

    //reciveMSG
    sockaddr_in clientAddr;
    SOCKET clientSocket;
public:
    Host() {
        CreatePort();
    }

    int CreatePort() {
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            std::cerr << "WSAStartup failed\n";
            return 1;
        }

        serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (serverSocket == INVALID_SOCKET) {
            std::cerr << "Error creating socket: " << WSAGetLastError() << "\n";
            WSACleanup();
            return 1;
        }

        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(12345); // Choose a port
        serverAddr.sin_addr.s_addr = INADDR_ANY;

        if (bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR) {
            std::cerr << "Error binding socket: " << WSAGetLastError() << "\n";
            closesocket(serverSocket);
            WSACleanup();
            return 1;
        }

        if (listen(serverSocket, 1) == SOCKET_ERROR) // second argument decides limit of clients
        {
            std::cerr << "Error listening on socket: " << WSAGetLastError() << "\n";
            closesocket(serverSocket);
            WSACleanup();
            return 1;
        }

        std::cout << "Server listening for incoming connections...\n";
    }

    int conectClient() {
        int clientAddrSize = sizeof(clientAddr);
        clientSocket = accept(serverSocket, reinterpret_cast<sockaddr*>(&clientAddr), &clientAddrSize);
            if (clientSocket == INVALID_SOCKET) {
                std::cerr << "Error accepting connection: " << WSAGetLastError() << "\n";
                closesocket(serverSocket);
                WSACleanup();
                return 1;
            }
            else {
                std::cout << "client conected" << std::endl;
            }
    }

    int reciveMSG() {
        conectClient();

            char buffer[1024];
            int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
            if (bytesReceived == SOCKET_ERROR) {
                std::cerr << "Error receiving data: " << WSAGetLastError() << "\n";
                return 1;
            }
            else {
                buffer[bytesReceived] = '\0';
                std::cout << "Received message: \n" << buffer << "\n";
            }

        return 0;
    }

    ~Host() {
        closesocket(clientSocket);
        closesocket(serverSocket);
        WSACleanup();
    }


    /*int server() {
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            std::cerr << "WSAStartup failed\n";
            return 1;
        }

        SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (serverSocket == INVALID_SOCKET) {
            std::cerr << "Error creating socket: " << WSAGetLastError() << "\n";
            WSACleanup();
            return 1;
        }

        sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(12345); // Choose a port
        serverAddr.sin_addr.s_addr = INADDR_ANY;

        if (bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR) {
            std::cerr << "Error binding socket: " << WSAGetLastError() << "\n";
            closesocket(serverSocket);
            WSACleanup();
            return 1;
        }

        if (listen(serverSocket, 1) == SOCKET_ERROR) // second argument decides limit of clients
        {
            std::cerr << "Error listening on socket: " << WSAGetLastError() << "\n";
            closesocket(serverSocket);
            WSACleanup();
            return 1;
        }

        std::cout << "Server listening for incoming connections...\n";

        sockaddr_in clientAddr;
        int clientAddrSize = sizeof(clientAddr);
        SOCKET clientSocket = accept(serverSocket, reinterpret_cast<sockaddr*>(&clientAddr), &clientAddrSize);
        while (true) {
            if (clientSocket == INVALID_SOCKET) {
                std::cerr << "Error accepting connection: " << WSAGetLastError() << "\n";
                closesocket(serverSocket);
                WSACleanup();
                return 1;
            }
            else {
                std::cout << "client conected" << std::endl;
            }

            char buffer[1024];
            int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
            if (bytesReceived == SOCKET_ERROR) {
                std::cerr << "Error receiving data: " << WSAGetLastError() << "\n";
                break;
            }
            else {
                buffer[bytesReceived] = '\0';
                std::cout << "Received message: \n" << buffer << "\n";
            }
        }



        std::cout << "Press enter to exit program" << std::endl;
        std::cin.get();
        // send

        closesocket(clientSocket);
        closesocket(serverSocket);
        WSACleanup();

        return 0;
    }*/
};