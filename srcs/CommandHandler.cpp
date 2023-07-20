#include "CommandHandler.hpp"
#include "EventHandler.hpp"

CommandHandler& CommandHandler::getInstance(){
	static CommandHandler instance;
	return instance;
}

CMD::CODE CommandHandler::identifyCommand(const std::string& cmd){
    const std::string title[CMD::SIZE] = {"CAP", "QUIT", "NICK", "JOIN", "KICK", "INVITE", "TOPIC", "MODE", "PART"};

    for(int i = 0; i < CMD::SIZE; i++){
        if(cmd == title[i])
            return static_cast<CMD::CODE>(i);
    }
    return CMD::NONE;
}

void CommandHandler::executeCommand(CMD::CODE cmdCode, std::vector<std::string>& parameters){
    //int errCode = //함수 호출, 성공시 0, 실패시 errCode
    if(cmdCode == CMD::NONE)
        return ;

	_eventHandler = &EventHandler::getInstance();
	_client = _eventHandler->getRequestClient();
	_channelManager = &ChannelManager::getInstance();
	_clientManager = &ClientManager::getInstance();

    typedef NUMERIC::CODE (CommandHandler::*FP)(std::vector<std::string>&);
    FP funcs[CMD::SIZE] = {&CommandHandler::cap, &CommandHandler::quit, &CommandHandler::nick, 
			&CommandHandler::join, &CommandHandler::kick, &CommandHandler::invite,
			&CommandHandler::topic, &CommandHandler::mode , &CommandHandler::part};
    
	//[호출자]![유저이름]@[ip주소] [커맨드] 만들기 : makeMessage() -> serverMessage or userMessage or *fp
	//
	
	(this->*funcs[static_cast<int>(cmdCode)])(parameters);
}

NUMERIC::CODE CommandHandler::nick(std::vector<std::string>& parameters){
    std::string candidateNickname = parameters[0];
    bool isValid = true;

	std::cout << "NICK execute\n";
	
    if(parameters.size() == 0)
        return NUMERIC::NO_PARAM;
    if(parameters.size() != 1)
		isValid =  false;
        //  432 "<client> <nick> :Erroneus nickname"  (ERR_ERRONEUSNICKNAME )
    if(candidateNickname[0] == '#' || candidateNickname[0] == ':')
        return NUMERIC::INVALID_NICK;
	
    Client *foundDuplicate = _clientManager->getClientByNick(candidateNickname);
	std::cout << _client << " request client address\n";
    if(foundDuplicate){
        if (foundDuplicate == _client)
			return NUMERIC::NOTHING;
		else
			return NUMERIC::DUP_NICK;
            //433 "<client> <nick> :Nickname is already in use" (ERR_NICKNAMEINUSE)  
    }
	

    if(candidateNickname.length() > CONFIG::NICKLEN)
        return NUMERIC::INVALID_NICK;

    _client->setNickName(candidateNickname);
	_clientManager->eraseClientByNick(_client->getNickName());
	_clientManager->insertClientByNick(candidateNickname, _client);
	
    return NUMERIC::SUCCESS;
}

NUMERIC::CODE CommandHandler::join(std::vector<std::string>& parameters){
    bool alreadyExists = false;
    bool isValid = true;
    std::queue<std::string> channelList;
    std::queue<std::string> keyList;
	std::string password;
	
	std::cout << "JOIN executed\n";
	
	if(parameters.empty())
		return NUMERIC::NO_PARAM;
	
    //#chnannel, #chanel#,chanel key,key,key
	parseByDelimeter(',', parameters[0], channelList);
	std::cout << "channel list size: " << channelList.size() << "\n";
	if (parameters.size() >= 2)
		parseByDelimeter(',', parameters[1], keyList);
	
    while (channelList.empty() == false){
        std::string channelName = channelList.front();
        if(channelName[0] != CHANNEL_PREFIX)
            return NUMERIC::BAD_CHAN_MASK;
		channelName.erase(0,1);
        Channel *foundChannel = _channelManager->getChannelByName(channelName);
		
		if (!foundChannel) //channel already exists
		{	
			if(channelName.find(7) != std::string::npos) //check invalid chann
				return NUMERIC::BAD_CHAN_MASK;// irc.local 476 two #asd.sdaf :Invalid channel name
			_channelManager->insertChannel(channelName, _client);
				// 생성 성공 메시지
		}
		else
		{
			if (!foundChannel->getInviteByNick(_client->getNickName()))
			{
				if(GET_PERMISSION_K(foundChannel->getPermissions())){//key option
					if(keyList.empty() == true || foundChannel->getPassword() != keyList.front())
					{
						return NUMERIC::BAD_CHAN_KEY;
						// "<client> <channel> :Cannot join channel (+k)"
					}					
					keyList.pop();
				}
				
				if(GET_PERMISSION_I(foundChannel->getPermissions())){ //i option
					
					//invitedList 확인
					if(!foundChannel->getInviteByNick(_client->getNickName()))
						return NUMERIC::INVITE_ONLY_CHAN;				
					
					//irc.local 473 three #qqq :Cannot join channel (invite only)
				}
			}
			
            if(foundChannel->isFull()) //user limit check
				return NUMERIC::FULL_CHANNEL;           
			
			//check
			foundChannel->insertClient(_client);
			Client * tempCheck= foundChannel->getClientByNick(_client->getNickName());
			std::cout << tempCheck->getNickName() << " must be same as nick requested to join\n";
			
			// 입장 성공 메시지
		}
		
        channelList.pop();

        // 토픽 출력
        // 유저 리스트 출력
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
    Channel *requestChannel;
    FSM& fsm = FSM::getInstance();
    
    std::cout << "MODE \n";
    
	if (parameters.size() < 2)
		return NUMERIC::NO_PARAM;

	channelName = parameters[0];
	options = parameters[1];

    std::cout << channelName << "\n";
    std::cout << options << "\n";
    if(channelName.front() != CHANNEL_PREFIX)
        return NUMERIC::UNKNOWN_ERR; //syntax error 400 : must specify #channelname.
    
    channelName.erase(0, 1); // PREFIX 지우기
    
    _eventHandler->setRequestChannel(_channelManager->getChannelByName(channelName));
    requestChannel = _eventHandler->getRequestChannel();
    
    if (requestChannel == NULL){
        std::cout << "No SUCH CHAN";
        return NUMERIC::NO_SUCH_CHAN;
    }

    for(int i = 2; i < parameters.size(); i++)
        params.push(parameters[i]);

	if (!requestChannel->getClientByNick(_client->getNickName()))
		return NUMERIC::NOT_ON_CHAN;

    if(requestChannel->getOperatorByNick(_client->getNickName()) == NULL)
		return NUMERIC::NOT_OPER;
		//error: 482 two #c :You must have channel op access or above to set channel mode k
        // "<client> <channel> :You're not channel operator"


    fsm.executeMode(params, options);
	// 반환된 에러 처리
    
    return NUMERIC::SUCCESS;
}

NUMERIC::CODE CommandHandler::part(std::vector<std::string>& parameters){
    std::queue<std::string> channelList;
	
	std::cout << "PART executed\n";
	
	if(parameters.empty())
		return NUMERIC::NO_PARAM;
	
    //#chnannel,#chanel,#channle
	parseByDelimeter(',', parameters[0], channelList);
	std::cout << "channel list size: " << channelList.size() << "\n";
	
    while (channelList.empty() == false){
        std::string channelName = channelList.front();
        if(channelName[0] != CHANNEL_PREFIX)
            return NUMERIC::BAD_CHAN_MASK;
		channelName.erase(0,1);
        Channel *foundChannel = _channelManager->getChannelByName(channelName);
		if (!foundChannel) 
			return NUMERIC::NO_SUCH_CHAN;	//#s :No such channel
		else
		{
			std::string clientName = _client->getNickName();
			if (!foundChannel->getClientByNick(clientName))
				return NUMERIC::NOT_ON_CHAN;	// #a :You're not on that channel
				
			foundChannel->eraseClient(clientName);
			foundChannel->eraseOperator(clientName);
			//part 성공 메시지
			//:two!root@ 127.0.0.1 PART :#myCh  → 채널의 모든 유저
		}		
        channelList.pop();
    }
	return NUMERIC::SUCCESS;
}

void CommandHandler::parseByDelimeter(char delimeter, std::string& parsingLine, std::queue<std::string> &buffer){
    std::stringstream ssLine(parsingLine);
    std::string word;

    while (std::getline(ssLine, word, delimeter))
        buffer.push(word);
}

NUMERIC::CODE CommandHandler::kick(std::vector<std::string>& parameters){
	//input : 	 KICK #channel user,user : msg
	std::string targetName;
	std::string channelName;
	Client* target;
	Channel *requestChannel;
	
	if(parameters.size() < 2)
		return NUMERIC::NOTHING;
	
	std::string channelName = parameters[0];
	if(channelName[0] != CHANNEL_PREFIX)
        return NUMERIC::BAD_CHAN_MASK;
	channelName.erase(0,1);

	//no such chann
	requestChannel = _channelManager->getChannelByName(channelName);
	if(requestChannel == NULL)
		return NUMERIC::NO_SUCH_CHAN;

	// no such nick
	targetName = parameters[1];
	if (!_clientManager->getClientByNick(targetName))
		return NUMERIC::NO_SUCH_NICK;

	// 시킨 사람이 채널에 있는지 확인 442 two #c :You're not on that channel!
	if (!requestChannel->getClientByNick(_client->getNickName()))
		return NUMERIC::NOT_ON_CHAN;

	// 시킨사람이 채널의 op인지 확인
	if (!requestChannel->getOperatorByNick(_client->getNickName()))
		return NUMERIC::NOT_OPER;
	
	// they're not on channel
	target = requestChannel->getClientByNick(targetName);
	if (target == NULL)
		return NUMERIC::NOT_ON_CHAN;

	requestChannel->eraseClient(targetName);
	requestChannel->eraseOperator(targetName);

	// 채널의 모든 유저에게 메시지 보내기
	
	return NUMERIC::SUCCESS;
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
	Client* target;
	Channel *requestChannel;
	
	if(parameters.size() < 2)
		return NUMERIC::NOTHING;
	
	std::string channelName = parameters[0];
	if(channelName[0] != CHANNEL_PREFIX)
        return NUMERIC::BAD_CHAN_MASK;
	channelName.erase(0,1);

	//no such chann
	requestChannel = _channelManager->getChannelByName(channelName);
	if(requestChannel == NULL)
		return NUMERIC::NO_SUCH_CHAN;

	// no such nick
	targetName = parameters[1];
	target = _clientManager->getClientByNick(targetName);
	if (!target)
		return NUMERIC::NO_SUCH_NICK;

	// 이미 채널에 해당 사용자가 있는가?
	if (requestChannel->getClientByNick(targetName))
		return NUMERIC::ALREADY_ON_CHAN;

	// 시킨 사람이 채널에 있는지 확인 442 two #c :You're not on that channel!
	if (!requestChannel->getClientByNick(_client->getNickName()))
		return NUMERIC::NOT_ON_CHAN;

	// 시킨사람이 채널의 op인지 확인
	if (!requestChannel->getOperatorByNick(_client->getNickName()))
		return NUMERIC::NOT_OPER;

	// invite 된 사용자는 key를 입력하지 않아도 됨
	requestChannel->insertInvite(target);

	// 채널의 모든 유저에게 메시지 보내기

	return NUMERIC::SUCCESS;
}

NUMERIC::CODE CommandHandler::topic(std::vector<std::string>& parameters){
	
	return NUMERIC::SUCCESS;
}

NUMERIC::CODE CommandHandler::cap(std::vector<std::string>& parameters){
    if (parameters.size() == 1 && parameters[0] == "LS"){
        //출력 .. 쫘르륵 : Channel, Client 에서 불러와서 define 된 값. 
    }
	return NUMERIC::SUCCESS;
}

NUMERIC::CODE CommandHandler::quit(std::vector<std::string>& parameters){
	
	return NUMERIC::SUCCESS;
}