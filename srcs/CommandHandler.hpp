#pragma once
#ifndef COMMAND_HANDLER_HPP
#define COMMAND_HANDLER_HPP

#include <string>
#include <vector>
#include <queue>
#include "ErrorHandler.hpp"
#include "ClientManager.hpp"
#include "ChannelManager.hpp"
#include "Enums.hpp"
#include "FSM.hpp"

class EventHandler;

class CommandHandler
{
private:
    Client *_client;
	EventHandler* _eventHandler;
	ChannelManager* _channelManager;
	ClientManager* _clientManager;

private:
	void parseByDelimeter(char delimeter, std::string& parsingLine, std::queue<std::string> &buffer);
	NUMERIC::CODE executeModeCommand(STATE::CODE &state, std::queue<std::string>& params, char c);
	NUMERIC::CODE cap(std::vector<std::string>& parameters);
	NUMERIC::CODE quit(std::vector<std::string>& parameters);
	NUMERIC::CODE nick(std::vector<std::string>& parameters);
	NUMERIC::CODE join(std::vector<std::string>& parameters);
	NUMERIC::CODE kick(std::vector<std::string>& parameters);
	NUMERIC::CODE invite(std::vector<std::string>& parameters);
	NUMERIC::CODE topic(std::vector<std::string>& parameters);
	NUMERIC::CODE mode(std::vector<std::string>& parameters);
	NUMERIC::CODE part(std::vector<std::string>& parameters);

public:
	static CommandHandler& getInstance();
	CMD::CODE identifyCommand(const std::string& cmd);
	void executeCommand(CMD::CODE cmdCode, std::vector<std::string>& parameters);
	//client가 요청 -> [어떤 클라이언트이ㅡ 요청인지 저장 : cur_event->ident ] -> parsing  -> 실행 (errorHandler , commandHandler 필요한 출력 : 저장한 fd로 출력)
};

#endif