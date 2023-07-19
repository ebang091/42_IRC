#pragma once
#ifndef MESSAGE_HANDLER_HPP
#define MESSAGE_HANDLER_HPP

#include <queue>
#include "Client.hpp"
#include "Enums.hpp"

class MessageHandler{
private:
    std::queue<std::string> _messageQ;

public:
	static MessageHandler& getInstance();
	
    void addRequestClientInfo(Client *client);
	void addCommand(CMD::CODE command, const std::string& option);
	void addMessage(const std::string& msg);
    void addError(NUMERIC::CODE errcode, const std::string& description);
};


#endif
