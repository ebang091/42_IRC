#pragma once
#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <map>
#include "Client.hpp"
#include "Enums.hpp"

class Channel{
private:
	std::string _name;
    std::string _topic;
	std::string _password;
	int 		_limit;
	char _permissions;
    time_t _creationTime;
	std::map<std::string, Client*> _clientList;
    std::map<std::string, Client*> _operators; //없으면 NULL

public:
    Channel();
	Channel(const std::string& channelName, Client* client);

public:
	const std::string& getName() const;
	const std::string& getTopic() const;
	const std::string& getPassword() const;
	char getPermissions() const;
	time_t getCreationTime() const;
	int getLimit() const;

	void setName(const std::string& name);
	void setTopic(const std::string& topic);
	void setPassword(const std::string& password);
	void setPermission(char permission);
	void setLimit(int n);

	Client* getOperatorByNick(const std::string& nickName) const;
	Client* getClientByNick(const std::string& nickName) const;
	void insertOperator(Client* client);
	void insertClient(Client* client);
	void eraseOperator(const std::string& nickName);
	void eraseClient(const std::string& nickName);
};

#endif