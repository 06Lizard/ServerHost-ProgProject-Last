#pragma once
#include <WinSock2.h>
#include "ClientHandler.h"
#include <string>

class TMPChat
{
private:
	int client;
	ClientHandler handler;
	ClientManager* manager;
public:
	TMPChat(int idx, ClientManager* manager);
private:
	std::future<void> snd();
};

