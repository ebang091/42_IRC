#pragma once
#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>

class Client{
public:
	Client(int socketNumber);

private:
    std::string _nickname;
    std::string _username;
    std::string _realname;
    int _socketNumber;

public:
    const std::string& getNickName() const;
	const std::string& getUserName() const;
	const std::string& getRealName() const;
	int getSocketNumber() const;
	
    void SetNickName(const std::string& newNickName);
    void SetUserName(const std::string& newUserName);
    void SetRealName(const std::string& newRealName);
};

#endif