#pragma once
#ifndef SOCKET_HANDLER_HPP
#define SOCKET_HANDLER_HPP
#define LISTEN_QUEUE_SIZE 15

#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include "ErrorHandler.hpp"

class SocketHandler{
private:
    int _serverSocket;

public:
    int getServerSocket();
    static SocketHandler& getInstance();
    void makeAndListenSocket(int portNumber);
};

#endif