#include "Channel.hpp"
#include "MessageHandler.hpp"

Channel::Channel(const std::string& channelName, Client* client)
	: _topic()
	, _name(channelName)
	, _password()
	, _notice("")
	, _welcomMsg("")
	, _limit(CONFIG::USERLIMIT)
	, _permissions(0)
{
	time(&_creationTime);
	Client* clientptr = NULL;
	_clientList.insert(std::make_pair(client->getNickName(), client));
	_operators.insert(std::make_pair(client->getNickName(), client));
	_clientList.insert(std::make_pair(BOT_NAME, clientptr));
	_operators.insert(std::make_pair(BOT_NAME, clientptr));

	_permissions = PERMISSION::T | PERMISSION::N;
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

Client* Channel::getInviteByNick(const std::string& nickName) const
{
	try{
		Client* find = _invitedList.at(nickName);
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

void Channel::insertInvite(Client* client)
{
	_invitedList.insert(std::make_pair(client->getNickName(), client));
}

const std::string& Channel::getName() const{
	return this-> _name;
}

const Channel::Topic& Channel::getTopic() const{
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

int Channel::getLimit() const{
	return this->_limit;
}

const std::string& Channel::getNotice() const{
	return _notice;
}

const std::string& Channel::getWelcomeMsg() const{
	return _welcomMsg;
}

void Channel::setNotice(const std::string& newNotice) {
	_notice = newNotice;
}

void Channel::setWelcomeMsg(const std::string& newWelcomeMsg){
	_welcomMsg = newWelcomeMsg;
}

void Channel::setName(const std::string& name){
	this->_name = name;	
}

void Channel::setTopic(const std::string& topic, Client *setUser){
	this->_topic.__content = topic;
	time(&(this->_topic.__creationTime));
	this->_topic.__setUser = setUser->getNickName() + "!" + setUser->getUserName() + "@" + setUser->getHost();
}

void Channel::setPassword(const std::string& password){
	this->_password = password;
}

void Channel::setPermission(char permission){
	this->_permissions = permission;
}

void Channel::setLimit(size_t n){
	_limit = n;
}

void Channel::eraseOperator(const std::string& nickName){
	_operators.erase(nickName);
}

int Channel::eraseClient(const std::string& nickName){
	_clientList.erase(nickName);
	return _clientList.size();
}

void Channel::eraseInvite(const std::string& nickName){
	_invitedList.erase(nickName);
}

bool Channel::isFull() const{
	if(this->_clientList.size() >= this->_limit)
		return true;
	return false;
}

void Channel::sendToClients(){
	MessageHandler& messageHandler = MessageHandler::getInstance();
	std::string msg = messageHandler.getBroadcastMsg();
	
	for (std::map<std::string, Client*>::iterator iter = _clientList.begin(); iter != _clientList.end(); ++iter){
		if (iter->second == NULL)
			continue;

		messageHandler.sendOrPushMessage(msg, iter->second);
		// ssize_t result = send(iter->second->getSocketNumber(), msg.c_str(), msg.length(), MSG_DONTWAIT);
		// if (result == -1)
		// 	result = 0;
		// else if (static_cast<size_t>(result) == msg.length())
		// 	continue;
	    // iter->second->sendQuePush(msg.substr(result, msg.size() - result));
	}
	messageHandler.flushOutput();
}

void Channel::sendToClients(std::set<int>& isSent){
	MessageHandler& messageHandler = MessageHandler::getInstance();
	std::string msg = messageHandler.getBroadcastMsg();

	if(_clientList.size() != 0) {
		for (std::map<std::string, Client*>::iterator iter = _clientList.begin(); iter != _clientList.end(); ++iter){
			if (iter->second == NULL)
				continue;
				
			int curFd = iter->second->getSocketNumber();
			if (isSent.find(curFd) != isSent.end())
				continue;

			isSent.insert(curFd);
			messageHandler.sendOrPushMessage(msg, iter->second);
			// ssize_t result = send(iter->second->getSocketNumber(), msg.c_str(), msg.length(), MSG_DONTWAIT);
			// if (result == -1)
			// 	result = 0;
			// else if (static_cast<size_t>(result) == msg.length())
			// 	continue;
			// iter->second->sendQuePush(msg.substr(result, msg.size() - result));
		}
	}
	messageHandler.flushOutput();
}

void Channel::getClientList(std::vector<std::string>& list)
{
	for (std::map<std::string, Client*>::iterator iter = _clientList.begin(); iter != _clientList.end(); ++iter){
		std::string elem = "";

		if (_operators.find(iter->first) != _operators.end())
			elem += "@";
		elem += iter->first.substr(0, iter->first.length());

		list.push_back(elem);
	}
}

void Channel::printClients()
{
	std::cout << " * operators \n";
	for (std::map<std::string, Client*>::iterator iter = _operators.begin(); iter != _operators.end(); ++iter)
		std::cout << "(" << iter->second->getNickName() << ") - ";
	std::cout << "\n";

	std::cout << " * clients \n";
	for (std::map<std::string, Client*>::iterator iter = _clientList.begin(); iter != _clientList.end(); ++iter)
		std::cout << "(" << iter->second->getNickName() << ") - ";
	std::cout << "\n";

	std::cout << " * invited \n";
	for (std::map<std::string, Client*>::iterator iter = _invitedList.begin(); iter != _invitedList.end(); ++iter)
		std::cout << "(" << iter->second->getNickName() << ") - ";
	std::cout << "\n";
}