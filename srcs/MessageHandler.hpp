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
#include "ErrorHandler.hpp"

class EventHandler;

class MessageHandler{
private:
    EventHandler *_eventHandler;  
    ClientManager *_clientManager;  
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
    char         _option;
    std::string _description;
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
    void sendMessage();
    void setCallerInfo();
    void setServerInfo(NUMERIC::CODE code);
    std::string ntoStr(int n);

public:
	static MessageHandler& getInstance();
    void flushOutput();

    const std::string& getBroadcastMsg();
    void setEventHandler(EventHandler *eventHandler);
    void setClientManager(ClientManager *ClientManager);
    void setRequestClientSocket(int socket);
    void setRequestClientInfo(const Client *client);
	void setCommand(const std::string& command);
    void setParam(const std::vector<std::string>& params);
	void setChannel(const std::string& channel);
    void setRplCode(NUMERIC::CODE code);
    void setReason(const std::string& reason);
    void setOption(const char option);
    void setTargetName(const std::string& targetName);
    void setDescription(const std::string &description);

    void setBroadCastMsg();

    void sendPartSuccess();
    void sendConnectionSuccess();
    void sendErrorUnknownError(const std::string& reason);
    void sendErrorWithCommand(NUMERIC::CODE code);
    void sendJoinSuccess();
    void sendNickSuccess(int clientSocket);
    void sendInviteSuccess();
    void sendKickSuccess(int clientSocket);
    void sendTopicSuccess();
    void sendErrorWithTargetUserAndChannel(NUMERIC::CODE code);
    void sendErrorWithNickAndTargetUserAndChannel();
    void sendErrorNoParam(NUMERIC::CODE code);
    void sendErrorWithChannel(NUMERIC::CODE code);
    void sendErrorWithNickAndTargetName(NUMERIC::CODE code);
    void sendInvalidModeError(NUMERIC::CODE code);
    void sendErrorWithCmdAndReason(NUMERIC::CODE code);
};
#endif
