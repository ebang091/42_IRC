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
#include "Bot.hpp"

class EventHandler;
class ClientManager;

class MessageHandler{
private:
    std::map<NUMERIC::CODE, std::string> codeMap;
    EventHandler	*_eventHandler;  
    ClientManager	*_clientManager;  
    int				_clientSocket;
    NUMERIC::CODE	_rplCode; //성공이거나 실패 시 저장

    std::string _userName;
    std::string _targetName;
    std::string _nickName;
    std::string _host;
    std::string _channel;
    std::string _command;
    std::string _reason;
    std::string _option;
    std::string _state;
    std::string _description;
    std::vector<std::string> _params;

    std::string _replyMsg;
    std::string _broadcastMsg;

private:
    MessageHandler();

private:
    void serializeChannelClientList();
    void sendMessage();
    void setCallerInfo();
    void setServerInfo(NUMERIC::CODE code);

public:
	static MessageHandler& getInstance();
    void flushOutput();
    std::string ntoStr(int n);
    std::string atoOption();
    std::string atoParam();

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
    void setOption(char option);
    void setState(STATE::CODE code);
    void setTargetName(const std::string& targetName);
    void setDescription(const std::string &description);

    void setBroadCastMsg();

    void sendInviteWithNickAndTargetUserAndChannel();
    void sendPrivMsgToUser();
    void sendPrivMsgToChannel(std::set<int>& isSet);
    void sendPartSuccess();
    void sendConnectionSuccess();
    void sendErrorUnknown(const std::string& reason);
    void sendErrorWithCommand(NUMERIC::CODE code);
    void sendJoinSuccess();
    void sendNickSuccess();
    void sendInviteSuccess();
    void sendKickSuccess();
    void sendQuitSuccess();
    void sendTopicSuccess();
    void sendModeSuccess();
    void sendPongMessage();
    void sendCapMessage();
    void sendErrorWithTargetUserAndChannel(NUMERIC::CODE code);
    void sendErrorWithNickAndTargetUserAndChannel();
    void sendErrorNoParam(NUMERIC::CODE code);
    void sendErrorWithChannel(NUMERIC::CODE code);
    void sendErrorWithNickAndTargetName(NUMERIC::CODE code);
    void sendInvalidModeError(NUMERIC::CODE code);
    void sendErrorNoModeParam();
    void sendErrorWithCmdAndReason(NUMERIC::CODE code);
};
#endif
