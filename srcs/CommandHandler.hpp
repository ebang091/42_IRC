#pragma once
#ifndef COMMAND_HANDLER_HPP
#define COMMAND_HANDLER_HP

#include <string>
#include <vector>
#include "ErrorHandler.hpp"
#include "ClientHandler.hpp"

namespace CMD{
	enum CODE{
		NONE = -1,
		CAP,
		QUIT,
		NICK,
		JOIN,
		KICK,
		INVITE,
		TOPIC,
		MODE,
		SIZE
	};
};

class CommandHandler
{
private:
	//fp [] = {Channel::function(), Client::function()}
	ERROR::CODE cap(std::vector<std::string>& parameters);
	ERROR::CODE quit(std::vector<std::string>& parameters);
	ERROR::CODE nick(std::vector<std::string>& parameters);
	ERROR::CODE join(std::vector<std::string>& parameters);
	ERROR::CODE kick(std::vector<std::string>& parameters);
	ERROR::CODE invite(std::vector<std::string>& parameters);
	ERROR::CODE topic(std::vector<std::string>& parameters);
	ERROR::CODE mode(std::vector<std::string>& parameters);

public:
	static CommandHandler& getInstance();
	CMD::CODE identifyCommand(const std::string& cmd);
	void executeCommand(CMD::CODE cmdCode, std::vector<std::string>& parameters);
	
	//client가 요청 -> [어떤 클라이언트이ㅡ 요청인지 저장 : cur_event->ident ] -> parsing  -> 실행 (errorHandler , commandHandler 필요한 출력 : 저장한 fd로 출력)
};

#endif