#include "MessageHandler.hpp"
#include "EventHandler.hpp"

MessageHandler::MessageHandler(){
	codeMap.insert(std::make_pair(NUMERIC::NO_PARAM, "You must specify a parameter for the mode. Syntax: <nick or key>"));
	codeMap.insert(std::make_pair(NUMERIC::NEED_MORE_PARAM, "Not enough parameters"));
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
	codeMap.insert(std::make_pair(NUMERIC::INVALID_MODE, "is unknown mode char to me"));
	codeMap.insert(std::make_pair(NUMERIC::INVITE_ONLY_CHAN, "Cannot join channel (invite-only)"));
	codeMap.insert(std::make_pair(NUMERIC::BAD_CHAN_MASK, "Invalid channel name"));
	codeMap.insert(std::make_pair(NUMERIC::WELCOME, "Welcome to the Localnet IRC Network ")); // 뒤에 three!root@127.0.0.1
	codeMap.insert(std::make_pair(NUMERIC::YOURHOST, "Your host is irc.local, running ft_irc"));
	codeMap.insert(std::make_pair(NUMERIC::CREATED, "This server was created ")); // 뒤에 01:23:10 Jul 24 2023
	codeMap.insert(std::make_pair(NUMERIC::MYINFO, "irc.local ft_irc iosw biklmnopstv :bklov"));
	codeMap.insert(std::make_pair(NUMERIC::ISUPPORT, ":are supported by this server")); // 앞에 AWAYLEN=200 CASEMAPPING=rfc1459 CHANLIMIT=#:20 CHANMODES=b,k,l,imnpst CHANNELLEN=64 CHANTYPES=# ELIST=CMNTU HOSTLEN=64 KEYLEN=32 KICKLEN=255 LINELEN=512 MAXLIST=b:100
	codeMap.insert(std::make_pair(NUMERIC::RPL_NAMREPLY, ""));
	codeMap.insert(std::make_pair(NUMERIC::RPL_ENDOFNAMES, "End of /NAMES list."));
}

MessageHandler& MessageHandler::getInstance(){
	static MessageHandler instance;
	return instance;
}
	
void MessageHandler::setRequestClientInfo(const Client *client){
	std::cout << "CLient addr in setREquestClientINfo : " << client << "\n";
	_userName = client->getUserName();
	_nickName = client->getNickName();
	_host = client->getHost();
}

void MessageHandler::setEventHandler(EventHandler *eventHandler){
	this->_eventHandler = eventHandler;
}

void MessageHandler::setRequestClientSocket(int socket){
	_clientSocket = socket;
}

void MessageHandler::setCommand(const std::string& command){
	_command = command;
}

const std::string& MessageHandler::getBroadcastMsg()
{
	return _broadcastMsg;
}

void MessageHandler::setOption(const std::string& option){
	_option = option;
}

void MessageHandler::setParam(const std::vector<std::string>& params){
	_params = params;
	// _params.clear();
	// _params.reserve(params.size());
	// for (int i = 0; i < params.size(); ++i)
	// 	_params.push_back(params[i]);
}

void MessageHandler::setChannel(const std::string& channel){
	if (!channel.empty() && channel.front() == CHANNEL_PREFIX)
		_channel = channel;
	else
		_channel = "#" + channel;
}

void MessageHandler::setRplCode(NUMERIC::CODE code){
	_rplCode = code;
	_reason = codeMap.find(code)->second;
}

void MessageHandler::setReason(const std::string& reason){
	_reason = reason;
}

void MessageHandler::setDescription(const std::string& description){
	_description = description;
}

void MessageHandler::setTargetName(const std::string& targetName){
	_targetName = targetName;
}

void MessageHandler::setBroadCastMsg()
{
	_broadcastMsg = _replyMsg;
}

void MessageHandler::serializeChannelClientList(){
	Channel *channel = EventHandler::getInstance().getRequestChannel();
	std::vector<std::string> list;
	std::stringstream ss;
	ss << NUMERIC::RPL_NAMREPLY;

	/*
	:irc.local 332 two #ch :topic
	:irc.local 333 two #ch one!root@127.0.0.1 :1690181082
	*/
	Channel::Topic topic = channel->getTopic();
	if (!topic.__content.empty()){
		std::stringstream ss2;		
		ss2 << NUMERIC::TOPIC;
		_replyMsg += ":irc.local " + ss2.str() + " " + _nickName + " " + _channel +  " :" + topic.__content + "\n";
		ss2.clear();

		ss2 << NUMERIC::TOPIC_WHOTIME;
		_replyMsg += ":irc.local " + ss2.str() + " ";
		ss2.clear();
		ss2 << topic.__creationTime;
		_replyMsg += _nickName + " " + _channel +  " " + topic.__setUser + " :" + ss.str() + "\n";
		// :irc.local 332 two #wwwwwww :hi
	}
	_replyMsg += ":irc.local " + ss.str() + " " + _nickName + " = " + _channel +  " :";

	channel->getClientList(list);	
	for (int i = 0; i < list.size(); ++i)
		_replyMsg += list[i] + " ";
	_replyMsg += "\n";
	//:irc.local 353 user = #ch :@hei ...
}

void MessageHandler::setServerInfo(NUMERIC::CODE code){
	std::stringstream ss;
	
	setRplCode(code);
	ss << _rplCode;

	_replyMsg += ":irc.local " + ss.str() + " ";
}

void MessageHandler::setCallerInfo(){
	_replyMsg += ":" +  _nickName + "!" + _userName + "@" + _host + " ";
}

NUMERIC::CODE MessageHandler::sendErrorWithTargetUserAndChannel(NUMERIC::CODE code){
	setServerInfo(code);
	_replyMsg += _targetName + " " + _channel + " :" + _reason + "\n";

	sendMessage();
	return code;
}

NUMERIC::CODE MessageHandler::sendErrorWithNickAndTargetName(NUMERIC::CODE code){
	setServerInfo(code);
	_replyMsg += _nickName + " " + _targetName + " :" + _reason + "\n";

	sendMessage();
	return code;
}

NUMERIC::CODE MessageHandler::sendErrorNoParam(NUMERIC::CODE code){
	setServerInfo(code);
	_replyMsg += _nickName + " " + _reason + "\n";

	sendMessage();
	return code;
}

NUMERIC::CODE MessageHandler::sendErrorUnknownError(const std::string& reason){
	setServerInfo(NUMERIC::UNKNOWN_ERR);
	_replyMsg += reason + "\n";

	sendMessage();
	return NUMERIC::UNKNOWN_ERR;
}

NUMERIC::CODE MessageHandler::sendErrorWithChannel(NUMERIC::CODE code){
	setServerInfo(code);
	_replyMsg += _nickName + " " +  _channel + " :" + _reason + "\n";

	sendMessage();
	return code;
}
//:irc.local NOTICE #ch :*** one invited two into the channel

NUMERIC::CODE MessageHandler::sendInviteSuccess(){
	_replyMsg += ":irc.local NOTICE " + _channel + " :*** " + _nickName + " invited " + _targetName + " into the channel";
	
	setBroadCastMsg();
	if(_eventHandler->getRequestClient() != NULL)
		_eventHandler->getRequestChannel()->sendToClients();
	return NUMERIC::SUCCESS;
}

NUMERIC::CODE MessageHandler::sendJoinSuccess(){
	setCallerInfo();
	_replyMsg += _command + " : " + _channel + "\n";
	
	serializeChannelClientList();
	sendErrorWithChannel(NUMERIC::RPL_ENDOFNAMES);
	if(_eventHandler->getRequestClient() != NULL)
		_eventHandler->getRequestChannel()->sendToClients();
	return NUMERIC::SUCCESS;
}

NUMERIC::CODE MessageHandler::sendNickSuccess(int clientSocket){
	setCallerInfo();
	_replyMsg += _command + " : ";

	for (int i = 0; i < _params.size(); ++i)
		_replyMsg += _params[i] + " ";
	_replyMsg += "\n";

	setBroadCastMsg();
	sendMessage();	
	
	std::set<int> isSet;
	isSet.insert(clientSocket);
	if(_eventHandler->getRequestChannel() != NULL)
		_eventHandler->getRequestChannel()->sendToClients(isSet);
	return NUMERIC::SUCCESS;
}

NUMERIC::CODE MessageHandler::sendKickSuccess(int clientSocket){
	setCallerInfo();
	_replyMsg += _command + " " + _channel + " " +  _targetName + " "  + _description + "\n";

	setBroadCastMsg();
	sendMessage();

	std::set<int> isSet;
	isSet.insert(clientSocket);
	if(_eventHandler->getRequestChannel() != NULL)
		_eventHandler->getRequestChannel()->sendToClients(isSet);
	return NUMERIC::SUCCESS;
}

NUMERIC::CODE MessageHandler::sendTopicSuccess(){
	//:ebang!root@127.0.0.1 TOPIC #n :aa
	setCallerInfo();
	_replyMsg += _command + " " + _channel + " " + _description + "\n";

	setBroadCastMsg();
	std::set<int> isSet;
	if(_eventHandler->getRequestChannel() != NULL)
		_eventHandler->getRequestChannel()->sendToClients(isSet);
	return NUMERIC::SUCCESS;
}

void MessageHandler::sendMessage(){
	if (send(_clientSocket, _replyMsg.c_str(), _replyMsg.length(), MSG_DONTWAIT) == -1)
		throw ErrorHandler::SendException();
}

void MessageHandler::flushOutput(){
	_replyMsg.clear();
	_broadcastMsg.clear();
}