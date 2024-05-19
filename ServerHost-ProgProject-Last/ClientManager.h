#pragma once
#include <unordered_map>
#include <string>
#include <winsock2.h>
#include <mutex>
#include <algorithm>
#include <cctype>

class ClientManager {
private:
    std::unordered_map<std::string, SOCKET> clients;  // Stores client sockets
    std::unordered_map<std::string, std::string> passwords; // Stores client passwords
    std::mutex mtx; // Mutex for thread safety

public:
    /// Adds a client with the given identifier and password
    void AddClient(const std::string& id, SOCKET socket, const std::string& password);

    /// Removes a client with the given identifier
    void RemoveClient(const std::string& id);

    /// Gets the socket associated with the client identifier
    SOCKET GetClientSocket(const std::string& id);

    /// Checks if a client exists with the given identifier
    bool ClientExists(const std::string& id);

    /// Verifies if the given password matches the stored password for the client
    bool ValidatePassword(const std::string& id, const std::string& password);

    /// Converts a string to lowercase
    std::string str_tolower(std::string str);
};