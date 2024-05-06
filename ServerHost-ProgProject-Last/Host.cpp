#include "host.h"
#include "ClientHandler.h"
#include <thread>

Host::Host() {
    // initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize Winsock." << std::endl;
        return;
    }
    else
        std::cerr << "Sucsesefully initialized Winsock." << std::endl;

    // open the socet
    if (!socketManager.Start(12345)) // hardcoded port for now
    {
        std::cerr << "Failed to start server." << std::endl;
        WSACleanup();
        return;
    }
    else
        std::cerr << "Sucsesefully started server." << std::endl;

    HandleClients();
}


Host::~Host() {
    WSACleanup();
    std::cerr << "Closed server." << std::endl;
}

void Host::HandleClients() {
    while (true) {
        //could be good making this in to it's own function and thread
        SOCKET clientSocket = socketManager.AcceptClient();
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Error accepting client connection." << std::endl;
            continue;
        }
        else
            std::cerr << "Sucsesefully accepted client connection." << std::endl;

        //switching to asynd and having this in the class itself could be good
        std::thread clientThread([clientSocket]() // make a new thread for that client
        {
            ClientHandler handler(clientSocket);
            handler.ReceiveMessages(); // tells client handler to look for recived messages
        });
        clientThread.detach(); // Detach the thread to let it run independently
    }
}