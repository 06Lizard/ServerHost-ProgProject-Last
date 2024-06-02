#include "TMPChat.h"

TMPChat::TMPChat(int idx, ClientManager* manager) 
	: client(idx), handler(idx, manager), manager(manager)
{
	handler.SendMSG("Login successful", manager->GetClientSocket(client));
	snd();
	while (true) 
	{		
		handler.ReceiveMSG().get();
	}
}

std::future<void> TMPChat::snd() 
{
	return std::async(std::launch::async, [this]() -> void
		{
			std::string msg;
			while (true)
			{
				std::cin >> msg;
				handler.SendMSG(msg, manager->GetClientSocket(client));
			}
			return;
		});
}