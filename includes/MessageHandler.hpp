#pragma once
#ifndef MESSAGE_HANDLER_HPP
#define MESSAGE_HANDLER_HPP

#include <queue>
#include <string>
#include <sstream>
#include <cstring>
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
	Client			*_client;
    NUMERIC::CODE	_rplCode;
    int _targetClientSocket;

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
    void setCallerInfo();
    void setServerInfo(NUMERIC::CODE code);

public:
	static MessageHandler& getInstance();
    std::string ntoStr(int n);
    std::string atoOption();
    std::string atoParam();

    void flushOutput();
    void sendOrPushMessage(std::string& msg, Client* target);
	void sendRemainBuffer(Client* target);
    const std::string& getBroadcastMsg();
    void setEventHandler(EventHandler *eventHandler);
    void setClientManager(ClientManager *ClientManager);
    void setRequestClient(Client *client);
    void setTargetClientSocket(int fd);
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
    void sendErrorNickAndTargetUserAndChannel(NUMERIC::CODE code);
    void sendAndTargetUserAndChannel(NUMERIC::CODE code);
    void sendPrivMsgToUser(Client* target);
    void sendPrivMsgToChannel(std::set<int>& isSet);
    void sendPartSuccess();
    void sendConnectionSuccess();
    void sendErrorUnknown(const std::string& reason);
    void sendErrorWithCommand(NUMERIC::CODE code);
    void sendJoinSuccess();
    void sendNickSuccess();
    void sendInviteSuccess(Client* target);
    void sendKickSuccess();
    void sendQuitSuccess();
    void sendTopicSuccess();
    void sendModeSuccess();
    void sendPongMessage();
    void sendCapMessage();
    void sendErrorWithTargetUserAndChannel(NUMERIC::CODE code, Client* target);
    void sendErrorCallerTargetUserAndChannel();
    void sendErrorNoParam(NUMERIC::CODE code);
    void sendErrorWithChannel(NUMERIC::CODE code);
    void sendErrorWithChannelToTarget(NUMERIC::CODE code, Client* target);
    void sendErrorWithNickAndTargetName(NUMERIC::CODE code);
    void sendInvalidModeError(NUMERIC::CODE code);
    void sendErrorNoModeParam(const std::string& desctiption);
    void sendErrorWithCmdAndReason(NUMERIC::CODE code);
    
    void BotSetDescriptionByCode(NUMERIC::CODE code);
};
#endif
