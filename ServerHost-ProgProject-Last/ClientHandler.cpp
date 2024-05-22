#include "ClientHandler.h"

ClientHandler::ClientHandler(int idx, ClientManager* manager)
    : idx(idx), clientManager(manager), running(true) {}

ClientHandler::~ClientHandler() 
{
    closesocket(*clientManager->GetClientSocket(idx));
}

std::future<void> ClientHandler::ReceiveMSG() 
{
    char buffer[1024];
    int bytesReceived;

    while (running) 
    {
        bytesReceived = recv(*clientManager->GetClientSocket(idx), buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived > 0) 
        {
            buffer[bytesReceived] = '\0';
            std::string msg(buffer);
            ProcessMessage(msg);
        }
        else if (bytesReceived == 0)
        {
            PrintWarning("Lost connection to client.");
            break;
        }
        else
        {
            PrintError("Error receiving listening data. " + std::to_string(WSAGetLastError()));
            break;
        }
    }
}

void ClientHandler::ProcessMessage(const std::string& msg)
{
    // Expected format: "<recipient_id>:<message>"
    size_t delimiterPos = msg.find(':'); //find the first ':' in msg
    if (delimiterPos != std::string::npos) // if there was a ':'
    { 
        int rcvidx = clientManager->ID_FromUsr(clientManager->str_tolower(msg.substr(0, delimiterPos)));
        std::string actualMsg = msg.substr(delimiterPos + 1);

        if (clientManager->ClientExists(rcvidx))
        {
            SendMSG(actualMsg, clientManager->GetClientSocket(rcvidx));
        }
        else
        {
            SendMSG("Recipient not found. ", clientManager->GetClientSocket(idx));
        }
    }
    else 
    {
        SendMSG("Invalid message format received. ", clientManager->GetClientSocket(idx));
    }
}

std::future<int> ClientHandler::SendMSG(std::string msg, SOCKET* rcp)
{
    return std::async(std::launch::async, [this, msg, rcp]() -> int
    {
        if (send(*rcp, msg.c_str(), strlen(msg.c_str()), 0) == SOCKET_ERROR)
        {
            PrintError("Error sending data '" + std::to_string(WSAGetLastError()) + "' with the message '" + msg + "'");
            return WSAGetLastError();
        }
        else
        {
            PrintSuccessful("Successfully sent message '" + msg + "'");
            return 0;
        }
    });
}