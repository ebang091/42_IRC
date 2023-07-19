#pragma once
#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>
#include <arpa/inet.h>
#include <sys/socket.h>


class Client{
public:
	Client(int socketNumber);

private:
    std::string _nickname;
    std::string _username;
    std::string _realname;
    std::string _ipAddress;
    int _socketNumber;

public:
    const std::string& getNickName() const;
	const std::string& getUserName() const;
	const std::string& getRealName() const;
    const std::string& getIpAddress() const;
	int getSocketNumber() const;

	
    void setNickName(const std::string& newNickName);
    void setUserName(const std::string& newUserName);
    void setRealName(const std::string& newRealName);
    void setIpAddress(int clientSocket);
};

#endif