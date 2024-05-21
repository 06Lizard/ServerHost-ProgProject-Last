#include "Host.h"

Host::Host() : running(true), threadPool(4) {
    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) 
    {
        PrintError("Failed to initialize Winsock.");
        return;
    }
    else 
    {
        PrintSuccessful("Successfully initialized Winsock.");
    }

    // Open the socket
    if (!socketManager.Start(12345)) { // hardcoded port for now
        PrintError("Failed to start server.");
        WSACleanup();
        return;
    }
    else 
    {
        PrintSuccessful("Successfully started server.");
    }

    // Handle clients in the main thread
    HandleClients();
}

Host::~Host() {
    running = false;
    socketManager.Close();
    WSACleanup();
    PrintWarning("Closed server.");
}

void Host::HandleClients() {
    while (running) {
        std::vector<SOCKET> clientSockets = socketManager.CheckEvents(); // list of all sockets with events

        for (SOCKET clientSocket : clientSockets) 
        {
            //std::cout << "for new" << std::endl;
            if (clientSocket == socketManager.GetListeningSocket()) 
            {
                //std::cout << "if" << std::endl;
                //Sleep(2000);
                AddClient();
                //std::cout << "if back" << std::endl; // client socket gets invalid or is straight up disconected as client always looses conection, selution could be to return a add client in here
                //Sleep(2000);
            }
        }

        for (int clientIdx : clientManager.GetClientIdxS()) 
        {
            //std::cout << "old for" << std::endl;
            //Sleep(2000);
            threadPool.enqueue([this, clientIdx]() { HandleClient(clientIdx); });
        }
    }
}

void Host::HandleClient(int clientIdx) 
{
    //std::cout << "H->HC" << std::endl;

    ClientHandler handler(clientIdx, &clientManager);
    handler.ReceiveMSG();
}

void Host::AddClient() 
{
    int idx = clientManager.AddClient(socketManager.AcceptClient());
    if (idx != INVALID_SOCKET) 
    {
        //threadPool.enqueue([this, newClientSocket]() {
        LoginClient loginClient(idx, &clientManager);
        // Post login, client socket is managed by ClientManager
        //});
    }
    else 
    {
        PrintError("Error logging in.");
        closesocket(*clientManager.GetClientSocket(idx));
        clientManager.RemoveClient(idx);
    }
} // as soon as this finishes the client disconects