#include "MessageHandler.hpp"

MessageHandler& MessageHandler::getInstance()
{
	static MessageHandler instance;
	return instance;
}

void MessageHandler::addMessage(const std::string& msg){
    //_messageQ.push(msg);
}

void MessageHandler::addRequestClientInfo(Client *client){
    
}