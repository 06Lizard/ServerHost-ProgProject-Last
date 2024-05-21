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
    /// <summary>
    /// Adds new clientSocket's to clientS
    /// </summary>
    /// <param name="clientSocket"></param>
    /// <returns></returns>
    int AddClient(SOCKET clientSocket);

    /// <summary>
    /// Add's username and password to the index of that client's spot in the clientS mapp
    /// </summary>
    /// <param name="username"></param>
    /// <param name="password"></param>
    /// <param name="idx"></param>
    void LoginClient(const std::string username, const std::string password, int idx);

    /// <summary>
    /// Removes the client in the map using the given index
    /// </summary>
    /// <param name="idx"></param>
    void RemoveClient(int idx);

    /// <summary>
    /// returns a refrence to the socket of the client asosiated with the that index
    /// </summary>
    /// <param name="idx"></param>
    /// <returns></returns>
    SOCKET* GetClientSocket(int idx);
    
    /// <summary>
    /// returns a list of all the client idx's
    /// </summary>
    /// <returns></returns>
    std::vector<int> GetClientIdxS();
    
    /// <summary>
    /// returns the idx assosiated with a username
    /// </summary>
    /// <param name="username"></param>
    /// <returns></returns>
    int ID_FromUsr(std::string username);

    /// <summary>
    /// Checks if the client at that idx exists and if so returns true, else false
    /// </summary>
    /// <param name="idx"></param>
    /// <returns></returns>
    bool ClientExists(int idx);

    /// <summary>
    /// checks if it's the corect password for that username
    /// </summary>
    /// <param name="id"></param>
    /// <param name="password"></param>
    /// <returns></returns>
    bool ValidatePassword(const std::string& username, const std::string& password);

    /// <summary>
    /// Converts a string to lowercase
    /// </summary>
    /// <param name="str"></param>
    /// <returns></returns>
    std::string str_tolower(std::string str);
};