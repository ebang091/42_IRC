#include "MessageHandler.hpp"
#include "EventHandler.hpp"
#include "Bot.hpp"

MessageHandler::MessageHandler()
: _targetClientSocket(0)
, _userName("")
, _targetName("")
, _nickName("")
, _host("")
, _channel("")
, _command("")
, _reason("")
, _option()
, _state("")
, _description("")
, _replyMsg("")
, _broadcastMsg("")
{
	// -- welcome
	codeMap.insert(std::make_pair(NUMERIC::WELCOME, " :Welcome to the Localnet IRC Network"));  
	codeMap.insert(std::make_pair(NUMERIC::INTRO, " :Your host is irc.local, running version ft_irc ver1.0"));
	codeMap.insert(std::make_pair(NUMERIC::SERVERCREATE, " :This server was created "));
	codeMap.insert(std::make_pair(NUMERIC::MYINFO, " :irc.local ft_irc iosw biklmnopstv :bklov"));
	codeMap.insert(std::make_pair(NUMERIC::CAPINFO, " :are supported by this server")); 
	codeMap.insert(std::make_pair(NUMERIC::USERINFO, "")); 
	codeMap.insert(std::make_pair(NUMERIC::CLIENTINFO, ""));
	codeMap.insert(std::make_pair(NUMERIC::MESSAGESTART, " :irc.local message of the day"));
	codeMap.insert(std::make_pair(NUMERIC::MESSAGEOFDAY, ""));
	codeMap.insert(std::make_pair(NUMERIC::MESSAGEEND, " :End of message of the day."));
	// --
	codeMap.insert(std::make_pair(NUMERIC::NO_PARAM, ""));
	codeMap.insert(std::make_pair(NUMERIC::INVITE, ""));
	codeMap.insert(std::make_pair(NUMERIC::UNKNOWN_CMD, "Unknown command"));
	codeMap.insert(std::make_pair(NUMERIC::NEED_MORE_PARAM, "Not enough parameters."));
	codeMap.insert(std::make_pair(NUMERIC::ALREADY_REGISTERED, "You may not reregister"));
	codeMap.insert(std::make_pair(NUMERIC::PASS_MISMATCH, "Password incorrect"));
	codeMap.insert(std::make_pair(NUMERIC::NO_SUCH_NICK, "No such nick"));
	codeMap.insert(std::make_pair(NUMERIC::INVALID_NICK, "Erroneous Nickname"));
	codeMap.insert(std::make_pair(NUMERIC::DUP_NICK, "Nickname is already in use"));
	codeMap.insert(std::make_pair(NUMERIC::NOT_OPER, "You must have channel op access or above to set channel mode"));
	codeMap.insert(std::make_pair(NUMERIC::NO_SUCH_CHAN, "No such channel"));
	codeMap.insert(std::make_pair(NUMERIC::TARGET_NOT_ON_CHAN, "They are not on that channel"));
	codeMap.insert(std::make_pair(NUMERIC::NOT_ON_CHAN, "You're not on that channel!"));
	codeMap.insert(std::make_pair(NUMERIC::ALREADY_ON_CHAN, "is already on channel"));
	codeMap.insert(std::make_pair(NUMERIC::BAD_CHAN_KEY, "Cannot join channel (incorrect channel key)"));
	codeMap.insert(std::make_pair(NUMERIC::KEY_ALREADY_SET, "Channel key already set"));
	codeMap.insert(std::make_pair(NUMERIC::FULL_CHANNEL, "Cannot join channel"));
	codeMap.insert(std::make_pair(NUMERIC::INVALID_MODE, "is not a recognised channel mode."));
	codeMap.insert(std::make_pair(NUMERIC::INVITE_ONLY_CHAN, "Cannot join channel (invite-only)"));
	codeMap.insert(std::make_pair(NUMERIC::BAD_CHAN_MASK, "Invalid channel name"));
	codeMap.insert(std::make_pair(NUMERIC::RPL_NAMREPLY, ""));
	codeMap.insert(std::make_pair(NUMERIC::RPL_ENDOFNAMES, "End of /NAMES list."));
	codeMap.insert(std::make_pair(NUMERIC::CANNOTSENDTOCHAN, "You cannot send external messages to this channel whilst the +n (noextmsg) mode is set."));
}

MessageHandler& MessageHandler::getInstance(){
	static MessageHandler instance;
	return instance;
}

void MessageHandler::setTargetClientSocket(int fd){
	_targetClientSocket = fd;
}
	
void MessageHandler::setRequestClient(Client *client){
	_client = client;
	_userName = client->getUserName();
	_host = client->getHost();

	_nickName.clear();
	const std::string& nickName = client->getNickName();
	for (size_t i = 0; i < nickName.length(); ++i){
		if (!std::isprint(nickName[i]))
			_nickName += ".";
		else
			_nickName += nickName[i];
	}
}

void MessageHandler::setEventHandler(EventHandler *eventHandler){
	this->_eventHandler = eventHandler;
}

void MessageHandler::setClientManager(ClientManager *clientManager){
	this->_clientManager = clientManager;
}

void MessageHandler::setCommand(const std::string& command){
	_command = command;
}

const std::string& MessageHandler::getBroadcastMsg(){
	return _broadcastMsg;
}

void MessageHandler::setOption(char option){
	_option = option;
}

void MessageHandler::setState(STATE::CODE code){
	_state = code == STATE::PLUS ? '+' : '-';
}

void MessageHandler::setParam(const std::vector<std::string>& params){
	_params = params;
}

void MessageHandler::setChannel(const std::string& channel){
	size_t i = 0;
	
	if (!channel.empty() && channel.front() == CHANNEL_PREFIX)
		++i;

	_channel = "#";
	for (; i < channel.length(); ++i){
		if (!std::isprint(channel[i]))
			_channel += ".";
		else
			_channel += channel[i];
	}
}

void MessageHandler::setRplCode(NUMERIC::CODE code){
	_rplCode = code;
	_reason = codeMap.find(code)->second;
}

void MessageHandler::BotSetDescriptionByCode(NUMERIC::CODE code){
	_rplCode = code;
	_description = "🍫 " + codeMap.find(code)->second;
}

void MessageHandler::setReason(const std::string& reason){
	_reason = reason;
}

void MessageHandler::setDescription(const std::string& description){
	if (description == "LS" || (!description.empty() && description.front() == DESCRIPT_PREFIX))
		_description = description;
	else
		_description = ":" + description;
}

void MessageHandler::setTargetName(const std::string& targetName){
	_targetName.clear();

	for (size_t i = 0; i < targetName.length(); ++i){
		if (!std::isprint(targetName[i]))
			_targetName += ".";
		else
			_targetName += targetName[i];
	}
}

void MessageHandler::setBroadCastMsg(){
	_broadcastMsg = _replyMsg;
}

void MessageHandler::serializeChannelClientList(){
	Channel *channel = EventHandler::getInstance().getRequestChannel();
	std::vector<std::string> list;
	std::stringstream ss;

	Channel::Topic topic = channel->getTopic();
	if (!topic.__content.empty()){
		_replyMsg += ":irc.local " + ntoStr(NUMERIC::TOPIC) + " " + _nickName + " " + _channel +  " " + topic.__content + "\n";
		
		_replyMsg += ":irc.local " +  ntoStr(NUMERIC::TOPIC_WHOTIME) + " ";
		
		_replyMsg += _nickName + " " + _channel +  " " + topic.__setUser + " :" + ntoStr(topic.__creationTime) + "\n";
	}
	_replyMsg += ":irc.local " + ntoStr(NUMERIC::RPL_NAMREPLY) + " " + _nickName + " = " + _channel +  " :";

	channel->getClientList(list);	
	for (size_t i = 0; i < list.size(); ++i)
		_replyMsg += list[i] + " ";
	_replyMsg += "\n";
}

void MessageHandler::setServerInfo(NUMERIC::CODE code){
	setRplCode(code);

	_replyMsg += ":irc.local " + ntoStr(_rplCode) + " ";
}

void MessageHandler::setCallerInfo(){
	_replyMsg += ":" +  _nickName + "!" + _userName + "@" + _host + " ";
}


void MessageHandler::sendErrorWithTargetUserAndChannel(NUMERIC::CODE code, Client* target){
	setServerInfo(code);
	_replyMsg += _targetName + " " + _channel + " :" + _reason + "\n";

	sendOrPushMessage(_replyMsg, target);
}

void MessageHandler::sendErrorCallerTargetUserAndChannel(){
	setCallerInfo();
	_replyMsg += _targetName + " " + _channel + " :" + _reason + "\n";

	sendOrPushMessage(_replyMsg, _client);
}

void MessageHandler::sendAndTargetUserAndChannel(NUMERIC::CODE code){
	setServerInfo(code);
	_replyMsg += _nickName + " " + _targetName + " :" + _channel + "\n";
	
	sendOrPushMessage(_replyMsg, _client);	
}

void MessageHandler::sendErrorNickAndTargetUserAndChannel(NUMERIC::CODE code){
	setServerInfo(code);
	_replyMsg += _nickName + " " + _targetName + " " + _channel +  " :" + _reason + "\n";
	
	sendOrPushMessage(_replyMsg, _client);	
}

void MessageHandler::sendErrorWithNickAndTargetName(NUMERIC::CODE code){
	setServerInfo(code);
	_replyMsg += _nickName + " " + _targetName + " :" + _reason + "\n";
	sendOrPushMessage(_replyMsg, _client);
}

void MessageHandler::sendErrorNoParam(NUMERIC::CODE code){
	setServerInfo(code);
	_replyMsg += _nickName + " :" + _reason + "\n";
	sendOrPushMessage(_replyMsg, _client);
}

void MessageHandler::sendErrorWithCommand(NUMERIC::CODE code){
	setServerInfo(code);
	_replyMsg += _nickName + " " + _command + " :" +  _reason + "\n";
	sendOrPushMessage(_replyMsg, _client);
}

void MessageHandler::sendErrorWithCmdAndReason(NUMERIC::CODE code){
	setServerInfo(code);
	_replyMsg += _command + " :" + _reason + "\n";
	sendOrPushMessage(_replyMsg, _client);
}

void MessageHandler::sendErrorUnknown(const std::string& reason){
	setServerInfo(NUMERIC::UNKNOWN_ERR);
	_replyMsg += reason + "\n";
	sendOrPushMessage(_replyMsg, _client);
}

void MessageHandler::sendErrorWithChannel(NUMERIC::CODE code){
	setServerInfo(code);
	_replyMsg += _nickName + " " +  _channel + " :" + _reason + "\n";
	sendOrPushMessage(_replyMsg, _client);
}

void MessageHandler::sendErrorWithChannelToTarget(NUMERIC::CODE code, Client* target){
	setServerInfo(code);
	_replyMsg += _nickName + " " +  _channel + " :" + _reason + "\n";
	sendOrPushMessage(_replyMsg, target);
}

void MessageHandler::sendInviteSuccess(Client* target){
	sendAndTargetUserAndChannel(NUMERIC::INVITE);

	_replyMsg += ":irc.local NOTICE " + _channel + " :*** " + _nickName + " invited " + _targetName + " into the channel\n";
	setBroadCastMsg();
	
	std::set<int> isSent;
	isSent.insert(_client->getSocketNumber());
	if(_eventHandler->getRequestChannel() != NULL)
		_eventHandler->getRequestChannel()->sendToClients(isSent);
	
	setCallerInfo();
	_replyMsg +=  _command + " " + _targetName + " :" + _channel + "\n";
	sendOrPushMessage(_replyMsg, target);
}

void MessageHandler::sendJoinSuccess(){
	setCallerInfo();
	_replyMsg += _command + " :" + _channel + "\n";
	setBroadCastMsg();

	serializeChannelClientList();
	sendErrorWithChannel(NUMERIC::RPL_ENDOFNAMES);

	std::set<int> isSent;
	isSent.insert(_client->getSocketNumber());
	if(_eventHandler->getRequestChannel() != NULL)
		_eventHandler->getRequestChannel()->sendToClients(isSent);

	Bot::getInstance().sendWelcomeMessage(_channel, _client);
}

void MessageHandler::sendQuitSuccess(){
	setCallerInfo();
	_replyMsg += _command + " " + _description + "\n";
	setBroadCastMsg();
}

void MessageHandler::sendNickSuccess(){
	setCallerInfo();
	_replyMsg += _command + " :" + _targetName + "\n";
	setBroadCastMsg();
	sendOrPushMessage(_replyMsg, _client);
}

void MessageHandler::sendKickSuccess(){
	setCallerInfo();
	_replyMsg += _command + " " + _channel + " " +  _targetName + " " + _description + "\n";
	setBroadCastMsg();
	if(_eventHandler->getRequestChannel() != NULL)
		_eventHandler->getRequestChannel()->sendToClients();
}

void MessageHandler::sendTopicSuccess(){
	setCallerInfo();
	_replyMsg += _command + " " + _channel + " " + _description + "\n";
	setBroadCastMsg();
	std::set<int> isSet;
	if(_eventHandler->getRequestChannel() != NULL)
		_eventHandler->getRequestChannel()->sendToClients(isSet);
}

void MessageHandler::sendModeSuccess(){
	setCallerInfo();
	_replyMsg += _command + " " + _channel;
	if (_description.empty())
		_replyMsg += " :" + _state + _option + "\n";
	else
		_replyMsg += " " + _state + _option + " " + _description + "\n";
	setBroadCastMsg();
	if(_eventHandler->getRequestChannel() != NULL)
		_eventHandler->getRequestChannel()->sendToClients();
}

void MessageHandler::sendInvalidModeError(NUMERIC::CODE code){
	setServerInfo(code);
	_replyMsg += _nickName + " " + _option +" :" + _reason + "\n";
	sendOrPushMessage(_replyMsg, _client);
}

std::string MessageHandler::atoOption(){
	if (_option == "k")
		return "key";
	else if (_option == "l")
		return "limit";
	else if (_option == "o")
		return "op";
	else
		return "error";
}

std::string MessageHandler::atoParam(){
	if (_option == "k")
		return "key";
	else if (_option == "l")
		return "limit";
	else if (_option == "o")
		return "nick";
	else
		return "error";	
}

void MessageHandler::sendErrorNoModeParam(const std::string& desctiption){
	setServerInfo(NUMERIC::NO_PARAM);
	_replyMsg += _nickName + " " + _channel + " " + _option + " ";

	if (desctiption.empty())
	{
		_replyMsg += "* :You must specify a parameter for the " + atoOption() + " mode.";
		_replyMsg += " Syntax: <" + atoParam() + ">.\n";
	}
	else
		_replyMsg += desctiption + "\n";
	sendOrPushMessage(_replyMsg, _client);
}

void MessageHandler::sendPrivMsgToUser(Client* target){
	setCallerInfo();
	_replyMsg += _command + " " + _targetName + " " + _description + "\n";
	sendOrPushMessage(_replyMsg, target);
}

void MessageHandler::sendPrivMsgToChannel(std::set<int>& isSent){
	setCallerInfo();
	_replyMsg += _command + " " + _channel + " " + _description + "\n";
	setBroadCastMsg();
	_eventHandler->getRequestChannel()->sendToClients(isSent);
}

void MessageHandler::sendOrPushMessage(std::string& msg, Client* target){
	std::queue<std::string>& sendQue = target->getSendQue();
	ssize_t result = 0;

	if (sendQue.empty()){
		result = send(target->getSocketNumber(), msg.c_str(), msg.length(), MSG_DONTWAIT);
		//std::cout << " send Msg : " << msg << "\n";
		if (result == -1)
			result = 0;
		else if (static_cast<size_t>(result) == msg.length()){
			return flushOutput();
		}
	}
	sendQue.push(msg.substr(result, msg.size() - result));
}

void MessageHandler::sendRemainBuffer(Client* target){
	std::queue<std::string>& sendQue = target->getSendQue();

	while (true){
		if (sendQue.empty())
			break;

		std::string& msg = sendQue.front();
		
		ssize_t result = send(target->getSocketNumber(), msg.c_str(), msg.length(), MSG_DONTWAIT);
		if (result == -1 || static_cast<size_t>(result) != msg.length()){
			if (result == -1)
				result = 0;
			sendQue.front() = msg.substr(result, msg.size() - result);
			break;
		}
		sendQue.pop();
	}
}

void MessageHandler::flushOutput(){
	_replyMsg.clear();
}

std::string MessageHandler::ntoStr(int n){
	std::stringstream ss;
	std::string ret;
	ss << n;
	ret = ss.str();
	while(ret.size() < 3)
		ret.insert(0, "0");
	return ret;
}

void MessageHandler::sendPartSuccess(){
	setCallerInfo();
	if (_description.empty())
		_replyMsg += _command + " :" + _channel + "\n";
	else
		_replyMsg += _command + " " + _channel + " " + _description + "\n";
	setBroadCastMsg();
	if(_eventHandler->getRequestChannel() != NULL)
		_eventHandler->getRequestChannel()->sendToClients();
}

void MessageHandler::sendConnectionSuccess(){
	const char MODES[CAP::MODESIZE] = {'k', 'l', 'o', 'i', 'n', 't'};
	const char CHANNELTYPES = '#';
	const char PREFIX = '@';

	setServerInfo(NUMERIC::WELCOME);
	_replyMsg = _nickName + ":" + _reason + "\n";
	
	setServerInfo(NUMERIC::INTRO);
	_replyMsg += _nickName + ":" + _reason + "\n";


	setServerInfo(NUMERIC::SERVERCREATE);
	_replyMsg += _nickName + ":" + _reason + "\n";


	setServerInfo(NUMERIC::CAPINFO);
	_replyMsg += _nickName;
	_replyMsg += " MODESIZE=" + ntoStr(CAP::MODESIZE) + " CHANNELLEN=" + ntoStr(CAP::CHANNELLEN) + " KEYLEN=" + ntoStr(CAP::KEYLEN) \
		+ " KICKLEN=" + ntoStr(CAP::KICKLEN) + " LINELEN=" +ntoStr(CAP::LINELEN) + " TOPICLEN=" + ntoStr(CAP::TOPICLEN) + " MODES=" + ntoStr(CAP::MODES) \
		+ " :are supported by this server\n";
	
	setServerInfo(NUMERIC::CAPINFO);
	_replyMsg += _nickName;
	_replyMsg += " USERLEN=" + ntoStr(CAP::USERLEN) + " HOSTLEN=" + ntoStr(CAP::HOSTLEN) + " MAXTARGETS=" + ntoStr(CAP::MAXTARGETS) + " NICKLEN=" + ntoStr(CAP::NICKLEN) \
	+ " CHANNELTYPES=" + CHANNELTYPES + " PREFIX=" + PREFIX + " MODES=";
	for(int i = 0 ; i < CAP::MODESIZE; i++)
		_replyMsg += MODES[i];
	_replyMsg += " :are supported by this server\n";

	setServerInfo(NUMERIC::USERINFO);
	int user = _clientManager->getClientNum();
	_replyMsg += _nickName + " :There are " + ntoStr(user) + " users and 001 invisible on 001 servers\n";

	setServerInfo(NUMERIC::MESSAGESTART);
	_replyMsg += _nickName + " " + _reason + "\n";

	setServerInfo(NUMERIC::MESSAGEOFDAY);
	_replyMsg += _nickName + MESSAGELINE1;

	setServerInfo(NUMERIC::MESSAGEOFDAY);
	_replyMsg += _nickName + MESSAGELINE2;

	setServerInfo(NUMERIC::MESSAGEOFDAY);
	_replyMsg += _nickName + MESSAGELINE3;

	setServerInfo(NUMERIC::MESSAGEOFDAY);
	_replyMsg += _nickName + MESSAGELINE4;

	setServerInfo(NUMERIC::MESSAGEOFDAY);
	_replyMsg += _nickName + MESSAGELINE5;
	
	setServerInfo(NUMERIC::MESSAGEOFDAY); 
	_replyMsg += _nickName + MESSAGELINE1;

	setServerInfo(NUMERIC::MESSAGEEND);
	_replyMsg += _reason + "\n";

	sendOrPushMessage(_replyMsg, _client);
	Client *client = _eventHandler->getRequestClient();
	client->setAuth(SWITCH_SENT_AUTH(client->getAuth()));
}

void MessageHandler::sendPongMessage(){
	_replyMsg += ":irc.local PONG irc.local " + _description + "\n";
	sendOrPushMessage(_replyMsg, _client);
}

void MessageHandler::sendCapMessage(){
	_replyMsg = _command + " * " + _description + "\n";
	sendOrPushMessage(_replyMsg, _client);
}
