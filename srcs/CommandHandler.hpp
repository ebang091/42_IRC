#pragma once
#ifndef COMMAND_HANDLER_HPP
#define COMMAND_HANDLER_HPP

#include <string>
#include <vector>
#include <queue>
#include <cstring>
#include "ErrorHandler.hpp"
#include "ClientManager.hpp"
#include "ChannelManager.hpp"
#include "Enums.hpp"
#include "FSM.hpp"

class MessageHandler;
class EventHandler;
class Parser;

class CommandHandler
{
private:
    Client *_client;
	EventHandler* _eventHandler;
	ChannelManager* _channelManager;
	ClientManager* _clientManager;
	Parser* _parser;
	MessageHandler* _messageHandler;

private:
	NUMERIC::CODE executeModeCommand(STATE::CODE &state, std::queue<std::string>& params, char c);
	bool getDescription(std::vector<std::string>& parameters, int startIdx, std::string& result);
	NUMERIC::CODE checkValid(std::string* channelName, const std::string* targetName, const std::string* callerName, bool checkOper);
	
	void quit(std::vector<std::string>& parameters);
	void nick(std::vector<std::string>& parameters);
	void join(std::vector<std::string>& parameters);
	void kick(std::vector<std::string>& parameters);
	void invite(std::vector<std::string>& parameters);
	void topic(std::vector<std::string>& parameters);
	void mode(std::vector<std::string>& parameters);
	void part(std::vector<std::string>& parameters);
	void privmsg(std::vector<std::string>& parameters);
	void user(std::vector<std::string>& parameters);
	void pass(std::vector<std::string>& parameters);

public:
	static CommandHandler& getInstance();
	CMD::CODE identifyCommand(const std::string& cmd);
	void executeCommand(CMD::CODE cmdCode, std::vector<std::string>& parameters);
	//client가 요청 -> [어떤 클라이언트이ㅡ 요청인지 저장 : cur_event->ident ] -> parsing  -> 실행 (errorHandler , commandHandler 필요한 출력 : 저장한 fd로 출력)
};

#endif