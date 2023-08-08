#pragma once
#ifndef BOT_HPP
#define BOT_HPP

#include <sstream>
#include <cstring>
#include <set>
#include "Client.hpp"
#include "Enums.hpp"

class CommandHandler;
class Channel;
class MessageHandler;
class ChannelManager;

class Bot
{
private:
    std::string _name;
	std::string _description;
    Client *_requestClient;
    Channel *_requestChannel;
    ChannelManager *_channelManager;
	MessageHandler *_messageHandler;

private:
	Bot();

private:
	BOT::CODE parseMessage(std::vector<std::string>& parameters);
	const std::string& getNotice() const;
	std::string getSendingMessage(BOT::CODE code);

public:
	static Bot& getInstance();
	const std::string& getName() const;
	void sendMessage(std::vector<std::string>& parameters, Client* requestClient);
    void sendWelcomeMessage(std::string channelName, Client* requestClient);
};

#endif