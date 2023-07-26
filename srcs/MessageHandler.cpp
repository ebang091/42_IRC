#include "MessageHandler.hpp"
#include "EventHandler.hpp"

MessageHandler::MessageHandler(){
	codeMap.insert(std::make_pair(NUMERIC::WELCOME, " :Welcome to the Localnet IRC Network"));
	codeMap.insert(std::make_pair(NUMERIC::INTRO, " :Your host is irc.local, running version kwsongeban_ver0.0"));
	codeMap.insert(std::make_pair(NUMERIC::SERVERCREATE, " :This server was created 10:59:44 Jul 18 2023"));
	codeMap.insert(std::make_pair(NUMERIC::MESSAGEEND, " :irc.local message of the day"));
	codeMap.insert(std::make_pair(NUMERIC::MESSAGEEND, " :End of message of the day."));
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
	codeMap.insert(std::make_pair(NUMERIC::INVALID_MODE, "is not a recognised channel mode."));
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

void MessageHandler::setOption(const char option){
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

	Channel::Topic topic = channel->getTopic();
	if (!topic.__content.empty()){
		_replyMsg += ":irc.local " + ntoStr(NUMERIC::TOPIC) + " " + _nickName + " " + _channel +  " :" + topic.__content + "\n";
		
		_replyMsg += ":irc.local " +  ntoStr(NUMERIC::TOPIC_WHOTIME) + " ";
		
		_replyMsg += _nickName + " " + _channel +  " " + topic.__setUser + " :" + ntoStr(topic.__creationTime) + "\n";
		
	}
	_replyMsg += ":irc.local " + ntoStr(NUMERIC::RPL_NAMREPLY) + " " + _nickName + " = " + _channel +  " :";

	channel->getClientList(list);	
	for (int i = 0; i < list.size(); ++i)
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

NUMERIC::CODE MessageHandler::sendInvalidModeError(NUMERIC::CODE code){
	//:irc.local 472 one x :is not a recognised channel mode.
	setServerInfo(code);
	_replyMsg += _nickName + " " + _option +" :" + _reason + "\n";
	sendMessage();

}

void MessageHandler::sendMessage(){
	if (send(_clientSocket, _replyMsg.c_str(), _replyMsg.length(), MSG_DONTWAIT) == -1)
		throw ErrorHandler::SendException();
	flushOutput();
}

void MessageHandler::flushOutput(){
	_replyMsg.clear();
	_broadcastMsg.clear();
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

void MessageHandler::sendConnectionSuccess(){

	// _clientManager->getClientByFD(_clientSocket)->setAuthenticated();
	char MODES[CAP::MODESIZE] = {'k', 'l', 'o', 'i', 'n', 't'};
	char CHANNELTYPES = '#'; //('#' 모든 서버가 알고 있다는 의미.)
	char PREFIX = '@'; //(채널 특권을 표현하는 문자. @: operator && creator)
	setServerInfo(NUMERIC::WELCOME);
	_replyMsg += _nickName + ":" + _reason + "\n";
	
	
	setServerInfo(NUMERIC::INTRO);
	_replyMsg += _nickName + ":" + _reason + "\n";
	sendMessage();

	setServerInfo(NUMERIC::SERVERCREATE);
	_replyMsg += _nickName + ":" + _reason + "\n";
	sendMessage();

	setServerInfo(NUMERIC::CAPINFO);
	_replyMsg += _nickName;
	_replyMsg += " MODESIZE=" + ntoStr(CAP::MODESIZE) + " CHANNELLEN=" + ntoStr(CAP::CHANNELLEN) + " KEYLEN=" + ntoStr(CAP::KEYLEN) \
		+ " KICKLEN=" + ntoStr(CAP::KICKLEN) + " LINELEN=" +ntoStr(CAP::LINELEN) + " TOPICLEN=" + ntoStr(CAP::TOPICLEN) + " MODES=" + ntoStr(CAP::MODES) \
		+" :are supported by this server\n";
	
	setServerInfo(NUMERIC::CAPINFO);
	_replyMsg += _nickName;
	_replyMsg += " USERLEN=" + ntoStr(CAP::USERLEN) + " HOSTLEN=" + ntoStr(CAP::HOSTLEN) + " MAXTARGETS=" + ntoStr(CAP::MAXTARGETS) + " NICKLEN=" + ntoStr(CAP::NICKLEN) \
	+ " CHANNELTYPES=" + CAP::CHANNELTYPES + " PREFIX=" + CAP::PREFIX + " MODES=";
	for(int i = 0 ; i < CAP::MODESIZE; i++)
		_replyMsg += MODES[i];
	_replyMsg += "  :are supported by this server\n";

	setServerInfo(NUMERIC::USERINFO);
	int user = _clientManager->getClientNum();
	_replyMsg += _nickName + " :There are " + ntoStr(user) + " users and 1 invisible on 1 servers\n";

	setServerInfo(NUMERIC::MESSAGESTART);
	_replyMsg += _reason + "\n";

	setServerInfo(NUMERIC::MESSAGEOFDAY);
	_replyMsg += _nickName + MESSAGELINE1;

	setServerInfo(NUMERIC::MESSAGEOFDAY);
	_replyMsg += _nickName + MESSAGELINE2;

	setServerInfo(NUMERIC::MESSAGEOFDAY);
	_replyMsg += _nickName + MESSAGELINE3;

	setServerInfo(NUMERIC::MESSAGEOFDAY);
	_replyMsg += _nickName + MESSAGELINE4;
	
	setServerInfo(NUMERIC::MESSAGEOFDAY); 
	_replyMsg += _nickName + MESSAGELINE1;

	setServerInfo(NUMERIC::MESSAGEEND);
	_replyMsg += _reason + "\n";

	sendMessage();
	Client *client = _eventHandler->getRequestClient();
	client->setAuth(SET_SENT_AUTH(client->getAuth()));
}
/*

:irc.local 001 three :Welcome to the Localnet IRC Network three!root@127.0.0.1
:irc.local 002 three :Your host is irc.local, running version InspIRCd-3
:irc.local 003 three :This server was created 03:59:44 Jul 17 2023
:irc.local 004 three irc.local InspIRCd-3 iosw biklmnopstv :bklov
:irc.local 005 three AWAYLEN=200 CASEMAPPING=rfc1459 CHANLIMIT=#:20 CHANMODES=b,k,l,imnpst CHANNELLEN=64 CHANTYPES=# ELIST=CMNTU HOSTLEN=64 KEYLEN=32 KICKLEN=255 LINELEN=512 MAXLIST=b:100 :are supported by this server
:irc.local 005 three MAXTARGETS=20 MODES=20 NAMELEN=128 NETWORK=Localnet NICKLEN=30 PREFIX=(ov)@+ SAFELIST STATUSMSG=@+ TOPICLEN=307 USERLEN=10 USERMODES=,,s,iow WHOX :are supported by this server
:irc.local 251 three :There are 0 users and 2 invisible on 1 servers
:irc.local 253 three 1 :unknown connections
:irc.local 254 three 3 :channels formed
:irc.local 255 three :I have 2 clients and 0 servers
:irc.local 265 three :Current local users: 2  Max: 2
:irc.local 266 three :Current global users: 2  Max: 2
:irc.local 375 three :irc.local message of the day
:irc.local 372 three : **************************************************
:irc.local 372 three : *             H    E    L    L    O              *
:irc.local 372 three : *  This is a private irc server. Please contact  *
:irc.local 372 three : *  the admin of the server for any questions or  *
:irc.local 372 three : *  issues.                                       *
:irc.local 372 three : **************************************************
:irc.local 372 three : *  The software was provided as a package of     *
:irc.local 372 three : *  Debian GNU/Linux <https://www.debian.org/>.   *
:irc.local 372 three : *  However, Debian has no control over this      *
:irc.local 372 three : *  server.                                       *
:irc.local 372 three : **************************************************
:irc.local 372 three : (The sysadmin possibly wants to edit </etc/inspircd/inspircd.motd>)
:irc.local 376 three :End of message of the day.

*/
