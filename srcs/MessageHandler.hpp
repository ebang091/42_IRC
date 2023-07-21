#pragma once
#ifndef MESSAGE_HANDLER_HPP
#define MESSAGE_HANDLER_HPP

#include <queue>
#include "Client.hpp"
#include "Enums.hpp"

class MessageHandler{
private:
    // std::queue<std::string> _messageQ;
    int targetSocket;
    int _errorCode; //성공이거나 실패 시 저장
    
    std::string _userInfo;//[nick]!host@ip
    std::string _command;
    std::string _errorReason;
    std::vector<std::string> _params;



    //userinfo
    //[nick]!host@ip
    
    //command 
    //reason
    //params

    //errormessage format
    //[servername] [errcode] [user] [command] : [reason]

    //channel message format
    //[nick]!host@ip [command] [params]

    //params 는 command 에 종속적.

    //example
    //[command] [params]

    //[NICK] [:newNickname]
    //[JOIN] [:#myCh]
    //[INVITE] 
    //[TOPIC] [#myCh : newTopic]
    //[MODE] [#a : +i]
    //[PART] [:#myCh]
    //[QUIT] [:Quit: msg]
    //

public:
	static MessageHandler& getInstance();
	
    void makeServerMessage();
	void makeUserMessage();
	
    void addRequestClientInfo(Client *client);
	void addCommand(CMD::CODE command, const std::string& option);
	void addMessage(const std::string& msg);
    void addError(NUMERIC::CODE errcode, const std::string& description);
};
#endif
