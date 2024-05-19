#include "ClientManager.h"

int ClientManager::AddClient(SOCKET clientSocket) {
    std::lock_guard<std::mutex> lock(mtx);
    if (clientSocket == INVALID_SOCKET) { return -1; }
    client clit;
    clit.clientsSocket = clientSocket;
    clientS[idx] = clit;
    idx++;
    return idx-1;
}

void ClientManager::LoginClient(const std::string username, const std::string password, int idx) {
    std::lock_guard<std::mutex> lock(mtx);
    clientS[idx].username = username;
    clientS[idx].password = password;               
}

void ClientManager::RemoveClient(int idx) {
    std::lock_guard<std::mutex> lock(mtx);
    clientS.erase(idx);    
}

SOCKET* ClientManager::GetClientSocket(int idx) {
    std::lock_guard<std::mutex> lock(mtx);
    auto it = clientS.find(idx);
    if (it != clientS.end()) {
        return &it->second.clientsSocket;
    }
    SOCKET invalid = INVALID_SOCKET;
    return &invalid; // Return invalid socket if client not found
}

std::vector<int> ClientManager::GetClientIdxS() {
    std::lock_guard<std::mutex> lock(mtx);
    std::vector<int> clientIdxS;

    for (auto& value : clientS) {
        clientIdxS.push_back(value.first);
    }

    return clientIdxS;
}

int ClientManager::ID_FromUsr(std::string username) {
    std::lock_guard<std::mutex> lock(mtx);
    for (auto& value : clientS) {
        if (value.second.username == username) {
            return value.first;
        }
    }
    return -1; // !(usr == found)
}

bool ClientManager::ClientExists(int idx) {
    std::lock_guard<std::mutex> lock(mtx);
    return clientS.find(idx) != clientS.end();    
}

bool ClientManager::ValidatePassword(const std::string& id, const std::string& password) {
    std::lock_guard<std::mutex> lock(mtx);
    /*auto it = passwords.find(id);
    return it != passwords.end() && it->second == password;*/
    return true;
}

std::string ClientManager::str_tolower(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) {
        return std::tolower(c);
        });
    return str;
}