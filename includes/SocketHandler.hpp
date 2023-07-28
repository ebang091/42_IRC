#pragma once
#ifndef SOCKET_HANDLER_HPP
#define SOCKET_HANDLER_HPP

#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include "ErrorHandler.hpp"
#include "Enums.hpp"

class SocketHandler{
private:
    int _serverSocket;

private:
    SocketHandler(){};

public:
    static SocketHandler& getInstance();
    int getServerSocket();
    void makeSocketAndListen(int portNumber);
};

#endif