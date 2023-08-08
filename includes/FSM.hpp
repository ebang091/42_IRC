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
class ClientManager;

class FSM{
private:
    Channel *_channel;
    Client *_client;
	ClientManager *_clientManager;
	EventHandler* _eventHandler;
	MessageHandler* _messageHandler;

private:
	FSM();

private:
	INPUT::CODE getInput(char c) const;
	void executeAndChangeState(STATE::CODE &state, std::queue<std::string>& params, char c);
	void plusN(std::queue<std::string>& params, STATE::CODE& state);
	void plusT(std::queue<std::string>& params, STATE::CODE& state);
	void plusI(std::queue<std::string>& params, STATE::CODE& state);
	void plusK(std::queue<std::string>& params, STATE::CODE& state);
	void plusO(std::queue<std::string>& params, STATE::CODE& state);
	void plusL(std::queue<std::string>& params, STATE::CODE& state);	
	void minusN(std::queue<std::string>& params, STATE::CODE& state);
	void minusT(std::queue<std::string>& params, STATE::CODE& state);
	void minusI(std::queue<std::string>& params, STATE::CODE& state);
	void minusK(std::queue<std::string>& params, STATE::CODE& state);
	void minusO(std::queue<std::string>& params, STATE::CODE& state);
	void minusL(std::queue<std::string>& params, STATE::CODE& state);
	void toPlus(std::queue<std::string>& params, STATE::CODE& state);
	void toMinus(std::queue<std::string>& params, STATE::CODE& state);
	
public:
	void setMessageHandler(MessageHandler *messagehandler);
    void executeMode(std::queue<std::string>& params, const std::string& options);
    static FSM& getInstance();
};

#endif