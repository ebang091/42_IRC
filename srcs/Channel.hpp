#pragma once
#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include <map>
#include <set>
#include <unistd.h>
#include "Client.hpp"
#include "Enums.hpp"
#include "ErrorHandler.hpp"

class Channel{
public:
	struct Topic {
	public:
	    std::string	__content;
		time_t 		__creationTime;
		std::string	__setUser;
	};

private:
	Topic		 _topic;
	std::string _name;
	std::string _password;
	size_t 		_limit;
	char _permissions;
    time_t _creationTime;
	std::map<std::string, Client*> _clientList;
    std::map<std::string, Client*> _operators;
	std::map<std::string, Client*> _invitedList;

public:
    Channel();
	Channel(const std::string& channelName, Client* client);

public:
	const Channel::Topic& getTopic() const;
	const std::string& getName() const;
	const std::string& getPassword() const;
	const std::string& getTopicSetUser() const;
	char getPermissions() const;
	time_t getCreationTime() const;
	time_t getTopicCreationTime() const;
	int getLimit() const;
	

	void setName(const std::string& name);
	void setTopic(const std::string& topic, Client *setUser);
	void setPassword(const std::string& password);
	void setPermission(char permission);
	void setLimit(size_t n);

	Client* getOperatorByNick(const std::string& nickName) const;
	Client* getClientByNick(const std::string& nickName) const;
	Client* getInviteByNick(const std::string& nickName) const;
	void insertOperator(Client* client);
	void insertClient(Client* client);
	void insertInvite(Client* client);
	void eraseOperator(const std::string& nickName);
	int eraseClient(const std::string& nickName);
	void eraseInvite(const std::string& nickName);
	
	bool isFull() const;
	void sendToClients();
	void sendToClients(std::set<int>& isSent);
	void changeAllNickname(std::set<int>& isSent, const std::string originname, const std::string newname);

	void getClientList(std::vector<std::string>& list);

public:
	void printClients();
};

#endif