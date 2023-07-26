#pragma once
#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "Enums.hpp"


class Client{
public:
	Client(int socketNumber);

private:
    std::string _nickname;
    std::string _username;
    std::string _realname;
    std::string _host;
    char _auth;
    int _socketNumber;

public:
    bool isAuth() const;
    bool getAuth() const;
    void setAuth(const char auth);
    
    const std::string& getNickName() const;
	const std::string& getUserName() const;
	const std::string& getRealName() const;
    const std::string& getHost() const;
	int getSocketNumber() const;
	
    void setNickName(const std::string& newNickName);
    void setUserName(const std::string& newUserName);
    void setRealName(const std::string& newRealName);
    void setHost(const std::string& newHost);

    bool checkHost(int clientSocket, const std::string& newHost);
};

#endif