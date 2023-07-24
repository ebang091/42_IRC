#pragma once
#ifndef MESSAGE_HANDLER_HPP
#define MESSAGE_HANDLER_HPP

#include <queue>
#include <string>
#include <sstream>
#include "Client.hpp"
#include "Enums.hpp"
#include "Channel.hpp"
#include "ClientManager.hpp"

class EventHandler;

class MessageHandler{
private:
    EventHandler *_eventHandler;    
    std::map<NUMERIC::CODE, std::string> codeMap;
    int           _clientSocket;
    NUMERIC::CODE _rplCode; //성공이거나 실패 시 저장

    std::string _userName;
    std::string _targetName;
    std::string _nickName;
    std::string _host;
    std::string _channel;
    std::string _command;
    std::string _reason;
    std::string _option;
    CMD::CODE _cmdCode;
    std::vector<std::string> _params;

    std::string _replyMsg;
    std::string _broadcastMsg;
    //CAP : 출력이 다... -> ???=
    //JOIN -> 채널 정보-> '\n' 추가하면서 한 줄로 만들기??=

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

private:
    MessageHandler();
    void serializeChannelClientList();

public:
	static MessageHandler& getInstance();
    void flushOutput();

    const std::string& getBroadcastMsg();
    void setEventHandler(EventHandler *eventHandler);
    void setRequestClientSocket(int socket);
    void setRequestClientInfo(const Client *client);
	void setCommand(const std::string& command);
    void setOption(const std::string& option);
    void setParam(const std::vector<std::string>& params);
	void setChannel(const std::string& channel);
    void setRplCode(NUMERIC::CODE code);
    void setReason(const std::string& reason);
    void setTargetName(const std::string& targetName);

    void setBroadCastMsg();

    NUMERIC::CODE sendJoinSuccess();
    NUMERIC::CODE sendNickSuccess(int clientSocket);
    NUMERIC::CODE sendError(NUMERIC::CODE code);
    NUMERIC::CODE sendErrorWithUser(NUMERIC::CODE code);
};
#endif
