#pragma once
#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <map>
#include "Client.hpp"

class Channel{
public:
		std::string _name;
        std::string _topic;
		std::string password;
    	short _permissions;
		std::map<std::string, Client*> _clientList;
        std::map<std::string, Client*> _operators; //없으면 NULL
        time_t _creationTime;

	public:
        Channel();
		Channel(const std::string& channelName, Client* client);
};

#endif