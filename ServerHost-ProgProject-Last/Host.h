#pragma once
#include "SocketManager.h"
#include <iostream>

class Host {
private:
    SocketManager socketManager;

public:
    Host();
    ~Host();

private:
    void HandleClients();
};