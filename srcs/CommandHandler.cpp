
#include "CommandHandler.hpp"
#include "EventHandler.hpp"
#include "Server.hpp"
#include "Parser.hpp"
#include "MessageHandler.hpp"


CommandHandler& CommandHandler::getInstance(){
	static CommandHandler instance;
	return instance;
}
CMD::CODE CommandHandler::identifyCommand(const std::string& cmd){
    const std::string title[CMD::SIZE] = {"QUIT", "NICK", "JOIN", "KICK", "INVITE", "TOPIC", "MODE", "PART", "PRIVMSG", "USER", "PASS"};

    for(int i = 0; i < CMD::SIZE; i++){
        if (strcasecmp(cmd.c_str(), title[i].c_str()) == 0){
			// push message
			_messageHandler = &MessageHandler::getInstance();
			_messageHandler->setCommand(cmd);
        	return static_cast<CMD::CODE>(i);
		}
	}
    return CMD::NONE;
}

void CommandHandler::executeCommand(CMD::CODE cmdCode, std::vector<std::string>& parameters){
	_eventHandler = &EventHandler::getInstance();
	_channelManager = &ChannelManager::getInstance();
	_clientManager = &ClientManager::getInstance();
	_parser = &Parser::getInstance();
	_client = _eventHandler->getRequestClient();
	_messageHandler->setRequestClientInfo(_client);
	_messageHandler->setEventHandler(_eventHandler);
	_messageHandler->setRequestClientSocket(_client->getSocketNumber());
	
	if(cmdCode == CMD::NONE && GET_SENT_AUTH(_client->getAuth()))
		return _messageHandler->sendErrorWithCommand(NUMERIC::UNKNOWN_CMD);

	//nickname!hostname@ip 먼저 저장
    typedef void (CommandHandler::*FP)(std::vector<std::string>&);
    FP funcs[CMD::SIZE] = {&CommandHandler::quit, &CommandHandler::nick, 
			&CommandHandler::join, &CommandHandler::kick, &CommandHandler::invite,
			&CommandHandler::topic, &CommandHandler::mode , &CommandHandler::part,
			&CommandHandler::privmsg, &CommandHandler::user, &CommandHandler::pass};

	FP authFuncs[CMD::SIZE] = {NULL, &CommandHandler::nick, 
			NULL, NULL, NULL,
			NULL, NULL , NULL,
			NULL, &CommandHandler::user, &CommandHandler::pass};

	if (GET_SENT_AUTH(_client->getAuth()))
		(this->*funcs[static_cast<int>(cmdCode)])(parameters); // //내부에서 명령어 별 메시지 작성
	else if (cmdCode != CMD::NONE && authFuncs[static_cast<int>(cmdCode)] != NULL){
		(this->*authFuncs[static_cast<int>(cmdCode)])(parameters);
		_client->setAuth(cmdCode);
	}
}

void CommandHandler::nick(std::vector<std::string>& parameters){
    std::string candidateNickname;

	std::cout << "NICK execute\n";

	if(parameters.empty() || !(GET_PASS_AUTH(_client->getAuth())) || !(GET_USER_AUTH(_client->getAuth())))
		return;
	
	if(!(GET_NICK_AUTH(_client->getAuth())))
		_client->setAuth(SET_NICK_AUTH(_client->getAuth()));

	if(_client->isAuth())
		_messageHandler->sendConnectionSuccess();

	candidateNickname = parameters[0];
	if(candidateNickname.size() > CAP::NICKLEN)
		candidateNickname = candidateNickname.substr(0, CAP::NICKLEN);

	_messageHandler->setTargetName(candidateNickname);
    if(candidateNickname.front() == '#' || candidateNickname.front() == ':')
		return _messageHandler->sendErrorWithNickAndTargetName(NUMERIC::INVALID_NICK);
	
    Client *foundDuplicate = _clientManager->getClientByNick(candidateNickname);

    if(foundDuplicate){
        if (foundDuplicate == _client)
			return;
		else
			return _messageHandler->sendErrorWithNickAndTargetName(NUMERIC::DUP_NICK);
    }

    if(candidateNickname.length() > CONFIG::NICKLEN)
        return _messageHandler->sendErrorWithNickAndTargetName(NUMERIC::INVALID_NICK);

	_messageHandler->sendNickSuccess();
	
	std::string originname = _client->getNickName();
	_clientManager->eraseClientByNick(_client->getNickName());
	_client->setNickName(candidateNickname);
	_clientManager->insertClientByNick(candidateNickname, _client);
	_channelManager->changeNickNameAllChannels(originname, _client);
}

void CommandHandler::join(std::vector<std::string>& parameters){
    std::queue<std::string> channelList;
    std::queue<std::string> keyList;
	std::string channelName;

	if(parameters.empty())
		return ;
	
	_parser->parseByDelimeter(',', parameters[0], channelList);
	
	if (parameters.size() >= 2)
		_parser->parseByDelimeter(',', parameters[1], keyList);
	
    while (channelList.empty() == false){
        channelName = channelList.front();
		channelList.pop();

		if(channelName.size() > CAP::CHANNELLEN)
			channelName = channelName.substr(0, CAP::CHANNELLEN);
		_messageHandler->setChannel(channelName);
		_eventHandler->setRequestChannel(_channelManager->getChannelByName(channelName.substr(1, channelName.size() -1)));
		NUMERIC::CODE code = checkValid(&channelName, NULL, NULL, false);
		
		if (code == NUMERIC::NO_SUCH_CHAN){
			if(channelName.find(7) != std::string::npos){//check invalid chann
				_messageHandler->sendErrorWithChannel(NUMERIC::BAD_CHAN_MASK);
				continue;
			}
			_channelManager->insertChannel(channelName, _client);
			_eventHandler->setRequestChannel(_channelManager->getChannelByName(channelName));
		}
		else if (code == NUMERIC::SUCCESS){  //channel already exists
			Channel* foundChannel = _eventHandler->getRequestChannel();

			if (!foundChannel->getInviteByNick(_client->getNickName())){
				if(GET_PERMISSION_K(foundChannel->getPermissions())){ // key option
					if(keyList.empty() == true || foundChannel->getPassword() != keyList.front()){
						_messageHandler->sendErrorWithChannel(NUMERIC::BAD_CHAN_KEY);
						continue;
					}					
					keyList.pop();
				}
				if(GET_PERMISSION_I(foundChannel->getPermissions())){ //i option
					//invitedList 확인
					if(!foundChannel->getInviteByNick(_client->getNickName())){
						_messageHandler->sendErrorWithChannel(NUMERIC::INVITE_ONLY_CHAN);
						continue;
					}
				}
			}
            if(foundChannel->isFull()){ //user limit check
				_messageHandler->sendErrorWithChannel(NUMERIC::FULL_CHANNEL);
				continue;
			}
			foundChannel->insertClient(_client);
			foundChannel->eraseInvite(_client->getNickName());
		}
		else{
			_messageHandler->sendErrorWithChannel(code);
			continue;
		}
		_messageHandler->sendJoinSuccess();
    }
}

void CommandHandler::mode(std::vector<std::string>& parameters){
	//input: MODE +inopt -kft user password
    //params:+inopt -kft user password

    //parameters[0]    #channel
    //parameters[1]     + options
    //parametrs[2]      - options

    //keys              이후
    std::string channelName;
    std::string options;
    std::queue<std::string> params;
    FSM& fsm = FSM::getInstance();
	fsm.setMessageHandler(_messageHandler);
    std::cout << "MODE executed\n";

	if (parameters.size() < 2)
		return _messageHandler->sendErrorNoParam(NUMERIC::NEED_MORE_PARAM);

	channelName = parameters[0];
	options = parameters[1];
    
	_messageHandler->setChannel(channelName);
    
	for(int i = 2; i < parameters.size(); i++)
        params.push(parameters[i]);

   	NUMERIC::CODE code = checkValid(&channelName, NULL, &_client->getNickName(), true);

	if (code != NUMERIC::SUCCESS)
		return _messageHandler->sendErrorWithChannel(code);

    fsm.executeMode(params, options);
	// 반환된 에러 처리?
}

void CommandHandler::part(std::vector<std::string>& parameters){
    std::queue<std::string> channelList;
	std::string channelName;

	std::cout << "PART executed\n";
	
	if(parameters.empty())
		return ;
	
    //#chnannel,#chanel,#channle
	_parser->parseByDelimeter(',', parameters[0], channelList);
	
	std::string description = "";
	if (!getDescription(parameters, 1, description))
		return _messageHandler->sendErrorNoParam(NUMERIC::NEED_MORE_PARAM);

	_messageHandler->setDescription(description);

    while (channelList.empty() == false){
        channelName = channelList.front();
		channelList.pop();

		NUMERIC::CODE code = checkValid(&channelName, NULL, &_client->getNickName(), false);
		_messageHandler->setChannel(channelName);

		if (code != NUMERIC::SUCCESS){
			_messageHandler->sendErrorWithChannel(code);
			continue;
		}
		Channel* foundChannel = _eventHandler->getRequestChannel();
		_messageHandler->sendPartSuccess();
		foundChannel->eraseOperator(_client->getNickName());
		if (foundChannel->eraseClient(_client->getNickName()) == 0)
			_channelManager->eraseChannel(foundChannel->getName());
    }
}

void CommandHandler::kick(std::vector<std::string>& parameters){
	//input : 	 KICK #channel user,user : msg
	std::string targetName;
	std::string channelName;
	Client* target;
	Channel *requestChannel;
	
	std::cout << "KICK execute\n";
	if(parameters.size() < 2)
		return;
	
	channelName = parameters[0];
	targetName = parameters[1];
	
	_messageHandler->setChannel(channelName);
	_messageHandler->setTargetName(targetName);
	
	NUMERIC::CODE code = checkValid(&channelName, &targetName, &_client->getNickName(), true);	
	if (code != NUMERIC::SUCCESS)
		return _messageHandler->sendErrorWithTargetUserAndChannel(code);
	requestChannel = _eventHandler->getRequestChannel();
	
	target = requestChannel->getClientByNick(targetName);
	if (target == NULL)
		return _messageHandler->sendErrorWithTargetUserAndChannel(NUMERIC::TARGET_NOT_ON_CHAN);

	std::string description = "";
	if (!getDescription(parameters, 2, description))
		return _messageHandler->sendErrorNoParam(NUMERIC::NEED_MORE_PARAM);
	
	if(description.size() > CAP::KICKLEN)
		description = description.substr(0, CAP::KICKLEN);
	_messageHandler->setDescription(description);

	_messageHandler->sendKickSuccess();
	requestChannel->eraseOperator(targetName);
	if (requestChannel->eraseClient(targetName) == 0)
		_channelManager->eraseChannel(requestChannel->getName());
}

void CommandHandler::invite(std::vector<std::string>& parameters){
	/*
      (NO) 채널 리스트에 타겟 유저 추가
      → :irc.local 341 [operator] [target] :#[channel]
      → :irc.local NOTICE #[channel] :*** [operator] invited [target] into the channel
      → :one!root@127.0.0.1 INVITE [target] :#[channel]
	*/
	std::string targetName;
	std::string channelName;
	
	if(parameters.size() < 2)
		return;
	
	targetName = parameters[0];
	channelName = parameters[1];

	_eventHandler->setRequestChannel(_channelManager->getChannelByName(channelName.substr(1, channelName.size()-1)));;
	_messageHandler->setTargetName(targetName);
	_messageHandler->setChannel(channelName);
	NUMERIC::CODE code = checkValid(&channelName, &targetName, &_client->getNickName(), true);
	
	if (code != NUMERIC::SUCCESS)
		return _messageHandler->sendErrorWithChannel(code);

	// 이미 채널에 해당 사용자가 있는가?
	//:irc.local 443 ebang two #ch :is already on channel
	if (_eventHandler->getRequestChannel()->getClientByNick(targetName))
		return _messageHandler->sendErrorWithNickAndTargetUserAndChannel();
	// invite 된 사용자는 key를 입력하지 않아도 됨
	_eventHandler->getRequestChannel()->insertInvite(_clientManager->getClientByNick(targetName));
	_messageHandler->sendInviteSuccess();
}

void CommandHandler::topic(std::vector<std::string>& parameters){
	//1. no such channel
	//2. channel 에 client 존재확인
	//3. channel 에 t옵션 확인
	//4. t 가 켜져있는데 op 이거나 t가 안켜져있으면 확인
	//5. message 저장 (공백 허용)
	std::string channelName;
	Channel *requestChannel;

	if(parameters.empty())
		return;
	
	channelName = parameters[0];

	NUMERIC::CODE code = checkValid(&channelName, NULL, &_client->getNickName(), true);
	requestChannel = _eventHandler->getRequestChannel();
	
	if ((code != NUMERIC::SUCCESS && code != NUMERIC::NOT_OPER) || //OPERATOR 가 아니고 t 옵션이 켜져있을 때
		(code == NUMERIC::NOT_OPER && GET_PERMISSION_T(requestChannel->getPermissions())))
		return _messageHandler->sendErrorWithChannel(code);
		//482 two #a :You do not have access to change the topic on this channel

	std::string topic = "";
	if (!getDescription(parameters, 1, topic))
		return _messageHandler->sendErrorWithChannel(NUMERIC::NEED_MORE_PARAM);

	if(topic.size() > CAP::TOPICLEN)
		topic = topic.substr(0, CAP::TOPICLEN);
	if (topic == requestChannel->getTopic().__content)
		return;

	requestChannel->setTopic(topic, _client);
	_messageHandler->setDescription(topic);
	// 채널의 모든 유저에게 메시지 보내기
	_messageHandler->sendTopicSuccess();
}

void CommandHandler::quit(std::vector<std::string>& parameters){
	std::string description = "leaving";

	if (!getDescription(parameters, 0, description))
		return;

	_messageHandler->setDescription(description);
	_messageHandler->sendQuitSuccess();

	throw ErrorHandler::QuitException();
}

void CommandHandler::pass(std::vector<std::string>& parameters){
	//USER setting 되어있으면 return 
	if(!(GET_PASS_AUTH(_client->getAuth())))
		return _messageHandler->sendErrorNoParam(NUMERIC::ALREADY_REGISTERED);

	std::cout << "!!!!! " << Server::getInstance().getPassword() << ", " << parameters[0] << "\n";
	if(parameters.empty() || Server::getInstance().getPassword() != parameters[0])
		return _messageHandler->sendErrorWithCmdAndReason(NUMERIC::PASS_MISMATCH);

	_client->setAuth(SET_PASS_AUTH(_client->getAuth()));
}

void CommandHandler::privmsg(std::vector<std::string>& parameters){
	std::string target;
	std::string description;
	
	if (parameters.size() < 2)
		return;//no param

	target = parameters[0];

	NUMERIC::CODE code = checkValid(&target, NULL, NULL, false);

	description = "";
	if (!getDescription(parameters, 1, description))
		return _messageHandler->sendErrorNoParam(NUMERIC::NEED_MORE_PARAM);
	_messageHandler->setDescription(description);

	if (code == NUMERIC::NO_SUCH_CHAN)
		return _messageHandler->sendErrorWithChannel(NUMERIC::NO_SUCH_CHAN);
	else if (code == NUMERIC::BAD_CHAN_MASK)
	{
		_messageHandler->setTargetName(target);
		if (!_clientManager->getClientByNick(target))
			return _messageHandler->sendErrorWithNickAndTargetName(NUMERIC::NO_SUCH_NICK);
		
		_messageHandler->setRequestClientSocket(_clientManager->getClientByNick(target)->getSocketNumber());	
		_messageHandler->sendPrivMsgToUser();
	}
	else 
	{
		_messageHandler->setChannel(target);
		if (!_eventHandler->getRequestChannel()->getClientByNick(_client->getNickName()))
			return _messageHandler->sendErrorWithChannel(NUMERIC::CANNOTSENDTOCHAN);
		
		_messageHandler->sendPrivMsgToChannel();
	}
}

//input 형식 USER <username> <username> <host> :<real name>
//사용 형식 :one!song@127.0.0.1  (nick!user@host)
void CommandHandler::user(std::vector<std::string>& parameters){
	if(parameters.empty())
		return;

	if(GET_USER_AUTH(_client->getAuth()))
		return _messageHandler->sendErrorNoParam(NUMERIC::ALREADY_REGISTERED); //"<client> :You may not reregister"
	
	_client->setAuth(SET_USER_AUTH(_client->getAuth()));
	if(_client->isAuth()) //000 110 111
		_messageHandler->sendConnectionSuccess();
	
	if (parameters.size() < 4)
		return _messageHandler->sendErrorNoParam(NUMERIC::NEED_MORE_PARAM);
	// irc.local 461 two USER :Not enough parameters.
	
	// irc.local 462 two :You may not reregister
	if (!_client->getUserName().empty())
		return _messageHandler->sendErrorNoParam(NUMERIC::ALREADY_REGISTERED); //"<client> :You may not reregister"
	if (parameters[0] != parameters[1])
		return _messageHandler->sendErrorUnknownError("user name not same");
	
	if (parameters[0].size() > CONFIG::USERLEN)
		parameters[0] = parameters[0].substr(0, CONFIG::USERLEN);
	
	if (!_client->checkHost(_client->getSocketNumber(), parameters[2]))
		return _messageHandler->sendErrorUnknownError("invalid host");
	
	std::string realName = "";
	if(!getDescription(parameters, 3, realName))
		return _messageHandler->sendErrorUnknownError("needs colon(:)");

	if(parameters[2].size() > CAP::HOSTLEN)
		parameters[2] = parameters[2].substr(CAP::HOSTLEN);
	_client->setUserName(parameters[0]);
	_client->setHost(parameters[2]);
	_client->setRealName(realName);
}

bool CommandHandler::getDescription(std::vector<std::string>& parameters, int startIdx, std::string& result){
	//std::cout << parameters[startIdx].front() << "\n";
	if (parameters.size() <= startIdx || parameters[startIdx].front() != ':')
		return false;
	
	for (int i = startIdx; i < parameters.size(); ++i){
		result += parameters[i];
		if (i != parameters.size() - 1)
			result += " ";
	}
	return true;
}

NUMERIC::CODE CommandHandler::checkValid(std::string* channelName, const std::string* targetName, const std::string* callerName, bool checkOper){
	Channel* requestChannel;

	if(channelName->empty() || channelName->front() != CHANNEL_PREFIX)
        return NUMERIC::BAD_CHAN_MASK;
	channelName->erase(0,1);

	_eventHandler->setRequestChannel(_channelManager->getChannelByName(*channelName));
    requestChannel = _eventHandler->getRequestChannel();
	if (!requestChannel)
        return NUMERIC::NO_SUCH_CHAN;

	if (targetName && !_clientManager->getClientByNick(*targetName))
		return NUMERIC::NO_SUCH_NICK;

	if (callerName)
	{
		if (!requestChannel->getClientByNick(*callerName))
			return NUMERIC::NOT_ON_CHAN;	// #a :You're not on that channel

		if (checkOper && !requestChannel->getOperatorByNick(*callerName))
			return NUMERIC::NOT_OPER;
	}
	return NUMERIC::SUCCESS;
}