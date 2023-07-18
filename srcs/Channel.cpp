#include "Channel.hpp"

Channel::Channel(){
    
}

Channel::Channel(const std::string& channelName, Client* client)
	: _name(channelName)
	, _topic()
	, _permissions(0)
{
	time(&_creationTime);
	_clientList.insert(std::make_pair(client->getNickName(), client));
	_operators.insert(std::make_pair(client->getNickName(), client));
}