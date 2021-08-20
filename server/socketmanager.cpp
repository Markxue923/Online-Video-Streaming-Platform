#include "socketmanager.h"

SocketManager::SocketManager()
{
}

SocketManager *SocketManager::instance = new SocketManager;

SocketManager *SocketManager::getInstance()
{
    return instance;
}
