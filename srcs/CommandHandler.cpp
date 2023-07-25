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
    const std::string title[CMD::SIZE] = {"CAP", "QUIT", "NICK", "JOIN", "KICK", "INVITE", "TOPIC", "MODE", "PART", "PRIVMSG", "USER", "PASS"};

    for(int i = 0; i < CMD::SIZE; i++){
        if(cmd == title[i])
		{
			// push message
			_messageHandler = &MessageHandler::getInstance();
			_messageHandler->flushOutput();
			_messageHandler->setCommand(cmd);
			// _messageHandler->setCommandCode(static_cast<CMD::CODE>(i));
        	return static_cast<CMD::CODE>(i);
		}
	}
    return CMD::NONE;
}

void CommandHandler::executeCommand(CMD::CODE cmdCode, std::vector<std::string>& parameters){
    //int errCode = //함수 호출, 성공시 0, 실패시 errCode 
	
    if(cmdCode == CMD::NONE)
        return ;
	_eventHandler = &EventHandler::getInstance();
	_channelManager = &ChannelManager::getInstance();
	_clientManager = &ClientManager::getInstance();
	_parser = &Parser::getInstance();
	_client = _eventHandler->getRequestClient();
	_messageHandler->setRequestClientInfo(_client);
	_messageHandler->setEventHandler(_eventHandler);
	_messageHandler->setRequestClientSocket(_client->getSocketNumber());
	
	//nickname!hostname@ip 먼저 저장
    typedef NUMERIC::CODE (CommandHandler::*FP)(std::vector<std::string>&);
    FP funcs[CMD::SIZE] = {&CommandHandler::cap, &CommandHandler::quit, &CommandHandler::nick, 
			&CommandHandler::join, &CommandHandler::kick, &CommandHandler::invite,
			&CommandHandler::topic, &CommandHandler::mode , &CommandHandler::part,
			&CommandHandler::privmsg, &CommandHandler::user};
    
	NUMERIC::CODE status = (this->*funcs[static_cast<int>(cmdCode)])(parameters);// //내부에서 명령어 별 메시지 작성
	// if(!(VERIFY_SUCCESS(status)));
		//error message from CommandHandler
		//PASS_MISMATCH 이면 연결 끊기.
		//NUMERIC::SEND_ERR  처리
}

NUMERIC::CODE CommandHandler::nick(std::vector<std::string>& parameters){
    std::string candidateNickname;
    bool isValid = true;

	std::cout << "NICK execute\n";
	//command에 NICK 저장

    if(parameters.empty())
        return NUMERIC::NOTHING;

	candidateNickname = parameters[0];

	_messageHandler->setTargetName(candidateNickname);
    if(candidateNickname.front() == '#' || candidateNickname.front() == ':')
		return _messageHandler->sendErrorWithUser(NUMERIC::INVALID_NICK);
	
    Client *foundDuplicate = _clientManager->getClientByNick(candidateNickname);

    if(foundDuplicate){
        if (foundDuplicate == _client)
			return NUMERIC::NOTHING;
		else
			return _messageHandler->sendErrorWithUser(NUMERIC::DUP_NICK);
            //433 "<client> <nick> :Nickname is already in use" (ERR_NICKNAMEINUSE)  
    }
	
    if(candidateNickname.length() > CONFIG::NICKLEN)
        return _messageHandler->sendErrorWithUser(NUMERIC::INVALID_NICK);
	//:three!root@127.0.0.1 NICK :ebang

	//send
	_messageHandler->setParam(parameters);
	if (_messageHandler->sendNickSuccess(_client->getSocketNumber()) == NUMERIC::SEND_ERR)
		return NUMERIC::SEND_ERR;

	_clientManager->eraseClientByNick(_client->getNickName());
	_client->setNickName(candidateNickname);
	_clientManager->insertClientByNick(candidateNickname, _client);
	
    return NUMERIC::SUCCESS;
}

NUMERIC::CODE CommandHandler::join(std::vector<std::string>& parameters){
    std::queue<std::string> channelList;
    std::queue<std::string> keyList;

	if(parameters.empty())
		return NUMERIC::NO_PARAM;
	
    //#chnannel, #chanel#,chanel key,key,key
	_parser->parseByDelimeter(',', parameters[0], channelList);
	
	if (parameters.size() >= 2)
		_parser->parseByDelimeter(',', parameters[1], keyList);
	
    while (channelList.empty() == false){
		
        std::string channelName = channelList.front();
		_messageHandler->setChannel(channelList.front());
		NUMERIC::CODE code = checkValid(&channelName, NULL, NULL, false);
		
		if (code == NUMERIC::NO_SUCH_CHAN)
		{	
			if(channelName.find(7) != std::string::npos){//check invalid chann
				_messageHandler->sendError(NUMERIC::BAD_CHAN_MASK);
				continue;
			}
			_channelManager->insertChannel(channelName, _client);
			_eventHandler->setRequestChannel(_channelManager->getChannelByName(channelName));
		}
		else if (code == NUMERIC::SUCCESS)  //channel already exists
		{
			Channel* foundChannel = _eventHandler->getRequestChannel();

			if (!foundChannel->getInviteByNick(_client->getNickName()))
			{
				if(GET_PERMISSION_K(foundChannel->getPermissions())){ // key option
					if(keyList.empty() == true || foundChannel->getPassword() != keyList.front()){
						_messageHandler->sendError(NUMERIC::BAD_CHAN_KEY);
						continue;
					}					
					keyList.pop();
				}
				if(GET_PERMISSION_I(foundChannel->getPermissions())){ //i option
					//invitedList 확인
					if(!foundChannel->getInviteByNick(_client->getNickName())){
						_messageHandler->sendError(NUMERIC::INVITE_ONLY_CHAN);
						continue;
					}
				}
			}
            if(foundChannel->isFull()){ //user limit check
				_messageHandler->sendError(NUMERIC::FULL_CHANNEL);
				continue;
			}
			foundChannel->insertClient(_client);
			foundChannel->eraseInvite(_client->getNickName());
		}
		else
		{
			_messageHandler->sendError(code);
			continue;
		}			
		
		//send to request client
		//try catch
		if (_messageHandler->sendJoinSuccess() == NUMERIC::SEND_ERR)
			return NUMERIC::SEND_ERR;
		//send to channel users (broadcast msg)
		
        channelList.pop();
    }

	return NUMERIC::SUCCESS;
}

NUMERIC::CODE CommandHandler::mode(std::vector<std::string>& parameters){
	
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
    
    std::cout << "MODE \n";
    
	if (parameters.size() < 2)
		return NUMERIC::NO_PARAM;

	channelName = parameters[0];
	options = parameters[1];
    
    for(int i = 2; i < parameters.size(); i++)
        params.push(parameters[i]);

   	NUMERIC::CODE code = checkValid(&channelName, NULL, &_client->getNickName(), true);

	if (code != NUMERIC::SUCCESS)
		return code;

    fsm.executeMode(params, options);
	// 반환된 에러 처리
    
    return code;
}

NUMERIC::CODE CommandHandler::part(std::vector<std::string>& parameters){
    std::queue<std::string> channelList;
	std::string channelName;

	std::cout << "PART executed\n";
	
	if(parameters.empty())
		return NUMERIC::NO_PARAM;
	
    //#chnannel,#chanel,#channle
	_parser->parseByDelimeter(',', parameters[0], channelList);
	
	std::string reason = "";
	if (!getReason(parameters, 1, reason))
		return NUMERIC::NO_PARAM;

    while (channelList.empty() == false){
        channelName = channelList.front();

		NUMERIC::CODE code = checkValid(&channelName, NULL, &_client->getNickName(), false);

		if (code != NUMERIC::SUCCESS)
			return code;
		
		Channel* foundChannel = _eventHandler->getRequestChannel();
		foundChannel->eraseOperator(_client->getNickName());
		if (foundChannel->eraseClient(_client->getNickName()) == 0)
			_channelManager->eraseChannel(foundChannel->getName());
		//part 성공 메시지 + reason
		//:two!root@ 127.0.0.1 PART :#myCh  → 채널의 모든 유저
        channelList.pop();
    }
	return NUMERIC::SUCCESS;
}

NUMERIC::CODE CommandHandler::kick(std::vector<std::string>& parameters){
	//input : 	 KICK #channel user,user : msg
	std::string targetName;
	std::string channelName;
	Client* target;
	Channel *requestChannel;
	
	if(parameters.size() < 2)
		return NUMERIC::NOTHING;
	
	channelName = parameters[0];
	targetName = parameters[1];

	NUMERIC::CODE code = checkValid(&channelName, &targetName, &_client->getNickName(), true);
	
	if (code != NUMERIC::SUCCESS)
		return code;

	// they're not on channel
	target = requestChannel->getClientByNick(targetName);
	if (target == NULL)
		return NUMERIC::NOT_ON_CHAN;

	std::string reason = "";
	if (!getReason(parameters, 2, reason))
		return NUMERIC::NO_PARAM;

	requestChannel->eraseClient(targetName);
	requestChannel->eraseOperator(targetName);
	// 채널의 모든 유저에게 메시지 보내기
	// reason 같이보내기  (one!root@127.0.0.1 KICK #a two : msg)
	
	return code;
}

NUMERIC::CODE CommandHandler::invite(std::vector<std::string>& parameters){
	/*
	1. 채널이 존재하는가?
      (YES)403 one #s :No such channel
      (NO) 다음으로 진행
	2. 유저가 존재하는가?
      (YES) 401 two das :No such nick
      (NO) 다음으로 진행
	3. 이미 채널에 해당 사용자가 있는가?
      (YES) 443 one three #q :is already on channel
      (NO) 다음으로 진행
	4. 요청자가 channel 에 존재하는가? 
	5. 요청 client가 channel의 operator 인가?  
      (YES) 482 two #q :You must be a channel operator
      (NO) 채널 리스트에 타겟 유저 추가
      → :irc.local 341 [operator] [target] :#[channel]
      → :irc.local NOTICE #[channel] :*** [operator] invited [target] into the channel
      → :one!root@127.0.0.1 INVITE [target] :#[channel]
	*/
	std::string targetName;
	std::string channelName;
	
	if(parameters.size() < 2)
		return NUMERIC::NOTHING;
	
	targetName = parameters[0];
	channelName = parameters[1];

	NUMERIC::CODE code = checkValid(&channelName, &targetName, &_client->getNickName(), true);
	std::cout << code << "\n";
	if (code != NUMERIC::SUCCESS)
		return code;

	// 이미 채널에 해당 사용자가 있는가?
	if (_eventHandler->getRequestChannel()->getClientByNick(targetName))
		return NUMERIC::ALREADY_ON_CHAN;
	std::cout << "1\n";
	// invite 된 사용자는 key를 입력하지 않아도 됨
	_eventHandler->getRequestChannel()->insertInvite(_clientManager->getClientByNick(targetName));
	std::cout << "2\n";
	// 채널의 모든 유저에게 메시지 보내기

	return code;
}

NUMERIC::CODE CommandHandler::topic(std::vector<std::string>& parameters){
	//1. no such channel
	//2. channel 에 client 존재확인
	//3. channel 에 t옵션 확인
	//4. t 가 켜져있는데 op 이거나 t가 안켜져있으면 확인
	//5. message 저장 (공백 허용)
	std::string channelName;
	Channel *requestChannel;

	if(parameters.empty())
		return NUMERIC::NOTHING;
	
	channelName = parameters[0];

	NUMERIC::CODE code = checkValid(&channelName, NULL, &_client->getNickName(), true);
	requestChannel = _eventHandler->getRequestChannel();
	
	if ((code != NUMERIC::SUCCESS && code != NUMERIC::NOT_OPER) ||
		(code == NUMERIC::NOT_OPER && GET_PERMISSION_T(requestChannel->getPermissions())))
		return code;
		//482 two #a :You do not have access to change the topic on this channel

	std::string topic = "";
	if (!getReason(parameters, 1, topic))
		return NUMERIC::NO_PARAM;
	
	requestChannel->setTopic(topic, _client);

	// 채널의 모든 유저에게 메시지 보내기
	//:one!root@127.0.0.1 TOPIC #a :topic

	return code;
}

NUMERIC::CODE CommandHandler::quit(std::vector<std::string>& parameters){
	std::string msg = "leaving";

	if (!getReason(parameters, 0, msg))
		return NUMERIC::NO_PARAM;
		
	_channelManager->eraseClientAllChannels(_client->getNickName());
	// _clientManager->eraseClientByNick(_client->getNickName());
	// _clientManager->eraseClientByFD(_client->getSocketNumber());

	return NUMERIC::SUCCESS;
}

NUMERIC::CODE CommandHandler::pass(std::vector<std::string>& parameters){
	//USER setting 되어있으면 return 
	if(!_client->getUserName().empty())
		return NUMERIC::NOTHING;

	if(parameters.empty() || Server::getInstance().getPassword() != parameters[0])
	{
		_clientManager->eraseClientByNick(_client->getNickName());
		_clientManager->eraseClientByFD(_client->getSocketNumber());
		return NUMERIC::PASS_MISMATCH;
	}
	return NUMERIC::SUCCESS;	
}

//PING CAP LS

NUMERIC::CODE CommandHandler::cap(std::vector<std::string>& parameters){
	if (parameters.size() != 1 || parameters[0] != "LS")
		return NUMERIC::NOTHING;
	
	

    //출력 .. 쫘르륵 : Channel, Client 에서 불러와서 define 된 값. 
	return NUMERIC::SUCCESS;
}

NUMERIC::CODE CommandHandler::privmsg(std::vector<std::string>& parameters){
	std::string target;
	std::string msg;
	
	if (parameters.size() < 2)
		return NUMERIC::NOTHING;

	target = parameters[0];

	NUMERIC::CODE code = checkValid(&target, NULL, NULL, false);

	if (!getReason(parameters, 1, msg))
		return NUMERIC::NO_PARAM;

	if (code == NUMERIC::NO_SUCH_CHAN)
		return code;
	else if (code != NUMERIC::BAD_CHAN_MASK)
	{
		// to channel clients
		// send msg
	}
	else
	{
		// user bubble
		if (!_clientManager->getClientByNick(target))
			return NUMERIC::NO_SUCH_NICK;
		// send msg
	}

	return NUMERIC::SUCCESS;
}

//input 형식 USER <username> <username> <host> :<real name>
//사용 형식 :one!song@127.0.0.1  (nick!user@host)
/*
- <real name> 은 마지막 파라미터여야 하는데, 그 이유는 스페이스바를 포함할 수 있기 때문임.
- <username>의 최대 길이는 USERLEN RPL_ISUPPORT 파라미터에 명시되어야 하고, 만약에 이것보다 길면 그냥 잘라서 저장
 최소 길이는 1이고 비어있으면 안된다. 만약 없으면 ERR_NEEDMOREPARAMS 를 보내야 한다. 

 - 이렇게  저장하고 난 후 USER 가 다시 들어온다면, ERR_ALREADYREGISTERED 을 제출한다. 
*/
// USER root root 127.0.0.1 :root
NUMERIC::CODE CommandHandler::user(std::vector<std::string>& parameters){
	if (parameters.size() != 4)
		return NUMERIC::NOTHING;
	
	//USER 사용여부 미리 확인	
	if (!_client->getUserName().empty())
		return NUMERIC::ALREADY_REGISTERED; //"<client> :You may not reregister"

	if (parameters[0] != parameters[1])
		return NUMERIC::UNKNOWN_ERR;

	if (parameters[0].size() > CONFIG::USERLEN)
		parameters[0] = parameters[0].substr(0, CONFIG::USERLEN);
	
	if (!_client->checkHost(_client->getSocketNumber(), parameters[2]))
		return NUMERIC::UNKNOWN_ERR; //UNKNOWN ("invalid host")
	
	std::string realName = "";
	if(!getReason(parameters, 3, realName))
		return NUMERIC::UNKNOWN_ERR;//"needs colon(:)"
	
	_client->setUserName(parameters[0]);	
	_client->setHost(parameters[2]);
	_client->setRealName(realName);
	return NUMERIC::SUCCESS;
}

bool CommandHandler::getReason(std::vector<std::string>& parameters, int startIdx, std::string& result){
	if (parameters.front().front() != ':')
		return false;
	
	parameters.front().erase(0, 1);
	for (int i = startIdx; i < parameters.size(); ++i){
		result += parameters[i];
		if (i != parameters.size() - 1)
			result += " ";
	}
	return true;
}

NUMERIC::CODE CommandHandler::checkValid(std::string* channelName, const std::string* targetName, const std::string* callerName, bool checkOper)
{
	Channel* requestChannel;

	if(channelName->empty() || channelName->front() != CHANNEL_PREFIX)
        return NUMERIC::BAD_CHAN_MASK;
	channelName->erase(0,1);

	_eventHandler->setRequestChannel(_channelManager->getChannelByName(*channelName));
    requestChannel = _eventHandler->getRequestChannel();
	if (!requestChannel)
        return NUMERIC::NO_SUCH_CHAN;

	if (targetName)
	{
		if (!_clientManager->getClientByNick(*targetName))
			return NUMERIC::NO_SUCH_NICK;
	}

	if (callerName)
	{
		if (!requestChannel->getClientByNick(*callerName))
			return NUMERIC::NOT_ON_CHAN;	// #a :You're not on that channel

		if (checkOper && !requestChannel->getOperatorByNick(*callerName))
			return NUMERIC::NOT_OPER;
	}
	return NUMERIC::SUCCESS;
}