#include "Channel.hpp"

Channel::Channel(){
    
}

Channel::Channel(const std::string& channelName, Client* client)
	: _name(channelName)
	, _topic("")
	,_limit(CONFIG::USERLIMIT)
	, _permissions(0)
{
	time(&_creationTime);
	_clientList.insert(std::make_pair(client->getNickName(), client));
	_operators.insert(std::make_pair(client->getNickName(), client));
}

Client* Channel::getOperatorByNick(const std::string& nickName) const
{
	try{
		Client* find = _operators.at(nickName);
		return find;
	}
	catch(std::exception &e){
		return NULL;
	}
}

Client* Channel::getClientByNick(const std::string& nickName) const
{
	try{
		Client* find = _clientList.at(nickName);
		return find;
	}
	catch(std::exception &e){
		return NULL;
	}
}

void Channel::insertOperator(Client* client)
{
	_operators.insert(std::make_pair(client->getNickName(), client));
}

void Channel::insertClient(Client* client)
{
	_clientList.insert(std::make_pair(client->getNickName(), client));
}

const std::string& Channel::getName() const{
	return this-> _name;
}

const std::string& Channel::getTopic() const{
	return this->_topic;
}

const std::string& Channel::getPassword() const{
	return this->_password;
}

char Channel::getPermissions() const {
	return this->_permissions;
}

time_t Channel::getCreationTime() const {
	return this->_creationTime;	
}

int Channel::getLimit() const
{
	return this->_limit;
}

void Channel::setName(const std::string& name){
	this->_name = name;	
}

void Channel::setTopic(const std::string& topic){
	this->_topic = topic;
}


void Channel::setPassword(const std::string& password){
	this->_password = password;
}

void Channel::setPermission(char permission){
	this->_permissions = permission;
}

void Channel::setLimit(int n){
	_limit = n;
}

void Channel::eraseOperator(const std::string& nickName){
	_operators.erase(nickName);
}

void Channel::eraseClient(const std::string& nickName){
	_clientList.erase(nickName);
}