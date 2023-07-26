#pragma once
#ifndef FSM_HPP
#define FSM_HPP
#include "Enums.hpp"
#include "EventHandler.hpp"
#include <queue>
#include <string>
#include <cstring>
#include <sstream>
#include "MessageHandler.hpp"

class EventHandler;

class FSM{
private:
    Channel *_channel;
    Client *_client;
	EventHandler* _eventHandler;
	MessageHandler* _messageHandler;

private:
	INPUT::CODE getInput(char c) const;
	NUMERIC::CODE executeAndChangeState(STATE::CODE &state, std::queue<std::string>& params, char c);
	
	NUMERIC::CODE plusN(std::queue<std::string>& params, STATE::CODE& state);
	NUMERIC::CODE plusT(std::queue<std::string>& params, STATE::CODE& state);
	NUMERIC::CODE plusI(std::queue<std::string>& params, STATE::CODE& state);
	NUMERIC::CODE plusK(std::queue<std::string>& params, STATE::CODE& state);
	NUMERIC::CODE plusO(std::queue<std::string>& params, STATE::CODE& state);
	NUMERIC::CODE plusL(std::queue<std::string>& params, STATE::CODE& state);

	NUMERIC::CODE minusN(std::queue<std::string>& params, STATE::CODE& state);
	NUMERIC::CODE minusT(std::queue<std::string>& params, STATE::CODE& state);
	NUMERIC::CODE minusI(std::queue<std::string>& params, STATE::CODE& state);
	NUMERIC::CODE minusK(std::queue<std::string>& params, STATE::CODE& state);
	NUMERIC::CODE minusO(std::queue<std::string>& params, STATE::CODE& state);
	NUMERIC::CODE minusL(std::queue<std::string>& params, STATE::CODE& state);
	
	NUMERIC::CODE toPlus(std::queue<std::string>& params, STATE::CODE& state);
	NUMERIC::CODE toMinus(std::queue<std::string>& params, STATE::CODE& state);

public:
	FSM();
	void setMessageHandler(MessageHandler *messagehandler);
    void executeMode(std::queue<std::string>& params, const std::string& options);
    static FSM& getInstance();
    
};

#endif