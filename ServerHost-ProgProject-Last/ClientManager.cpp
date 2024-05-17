#include "ClientManager.h"

void ClientManager::AddClient(const std::string& id, SOCKET socket, const std::string& password) {
    std::lock_guard<std::mutex> lock(mtx);
    clients[id] = socket;
    passwords[id] = password; // Store the password
}

void ClientManager::RemoveClient(const std::string& id) {
    std::lock_guard<std::mutex> lock(mtx);
    clients.erase(id);
    passwords.erase(id); // Remove the password
}

SOCKET ClientManager::GetClientSocket(const std::string& id) {
    std::lock_guard<std::mutex> lock(mtx);
    return clients.at(id);
}

bool ClientManager::ClientExists(const std::string& id) {
    std::lock_guard<std::mutex> lock(mtx);
    return clients.find(id) != clients.end();
}

bool ClientManager::ValidatePassword(const std::string& id, const std::string& password) {
    std::lock_guard<std::mutex> lock(mtx);
    auto it = passwords.find(id);
    if (it != passwords.end() && it->second == password) {
        return true;
    }
    return false;
}
