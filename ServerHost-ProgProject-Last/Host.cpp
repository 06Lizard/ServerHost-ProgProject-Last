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
            if (clientSocket == socketManager.GetListeningSocket()) // new client event
            {
                //std::cout << "listening socket" << std::endl;
                //SOCKET newClientSocket = socketManager.AcceptClient();
                //std::cout << "newClientSocket made" << std::endl;
                //if (newClientSocket != INVALID_SOCKET) {
                //    LoginClient loginClient(newClientSocket, &clientManager);
                //    std::cout << "back to host" << std::endl;//debuging for now
                //    Sleep(3000); //debuging for now
                //}
                //else 
                //{
                //    std::cerr << "Error loggin in" << std::endl;
                //    closesocket(newClientSocket);
                //    // make this run async
                //    //make sure client sockets that are in the login stage don't get sent to the threadpool below, exclude em from check event
                //}

                threadPool.enqueue([this]() { AddClient(); });

                std::cout << "ifel" << std::endl;//debuging for now
                Sleep(3000);//debuging for now

            }
            else // getMSG event
            {
                threadPool.enqueue([this, clientSocket]() { HandleClient(clientSocket); });
            }
            std::cout << "if" << std::endl;//debuging for now
            Sleep(3000);//debuging for now
        }
        std::cout << "for" << std::endl;//debuging for now
        Sleep(3000);//debuging for now
    }
}

void Host::HandleClient(SOCKET clientSocket) // acceppted clients
{
    std::cout << "H->HC" << std::endl;

    ClientHandler handler(clientSocket, &clientManager);
    handler.ReceiveMSG();
}

void Host::AddClient() // new clients
{ 
    // make this run async
    //make sure client sockets that are in the login stage don't get sent to the accepted clients, exclude em from check event

    std::cout << "listening socket" << std::endl;
    SOCKET newClientSocket = socketManager.AcceptClient();
    std::cout << "newClientSocket made" << std::endl;
    if (newClientSocket != INVALID_SOCKET) {
        LoginClient loginClient(newClientSocket, &clientManager);
    }
    else
    {
        std::cerr << "Error loggin in" << std::endl;
        closesocket(newClientSocket);
    }
}