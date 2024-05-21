#pragma once
#include <unordered_map>
#include <string>
#include <winsock2.h>
#include <mutex>
#include <algorithm>
#include <cctype>

class ClientManager
{
private:
    struct client
    {
        std::string username;
        std::string password;
        SOCKET clientsSocket;
    };

    int idx = 0; // add better indecks
    std::unordered_map<int, client> clientS;
    std::mutex mtx; // Mutex for thread safety

public:
    /// Adds a client with the given identifier and password
    int AddClient(SOCKET clientSocket);

    void LoginClient(const std::string username, const std::string password, int idx);

    /// Removes a client with the given identifier
    void RemoveClient(int idx);

    /// Gets the socket associated with the client identifier
    SOCKET* GetClientSocket(int idx);

    std::vector<int> GetClientIdxS();

    int ID_FromUsr(std::string username);

    /// Checks if a client exists with the given identifier
    bool ClientExists(int idx);

    /// Verifies if the given password matches the stored password for the client
    bool ValidatePassword(const std::string& id, const std::string& password);

    /// Converts a string to lowercase
    std::string str_tolower(std::string str);
};