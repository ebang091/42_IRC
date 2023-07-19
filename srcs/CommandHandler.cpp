#include "CommandHandler.hpp"
#include "EventHandler.hpp"

CommandHandler& CommandHandler::getInstance(){
	static CommandHandler instance;
	return instance;
}

CMD::CODE CommandHandler::identifyCommand(const std::string& cmd){
    const std::string title[CMD::SIZE] = {"CAP", "QUIT", "NICK", "JOIN", "KICK", "INVITE", "TOPIC", "MODE"};

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
    typedef NUMERIC::CODE (CommandHandler::*FP)(std::vector<std::string>&);
    FP funcs[CMD::SIZE] = {&CommandHandler::cap, &CommandHandler::quit, &CommandHandler::nick, 
			&CommandHandler::join, &CommandHandler::kick, &CommandHandler::invite,
			&CommandHandler::topic, &CommandHandler::mode};
    
	(this->*funcs[static_cast<int>(cmdCode)])(parameters);
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

NUMERIC::CODE CommandHandler::nick(std::vector<std::string>& parameters){
    ClientManager &clientHandler = ClientManager::getInstance();
    std::string candidateNickname = parameters[0];
    bool isValid = true;

    if(parameters.size() == 0)
        return NUMERIC::NO_PARAM;
    if(parameters.size() != 1)
		isValid =  false;
        //  432 "<client> <nick> :Erroneus nickname"  (ERR_ERRONEUSNICKNAME )
    if(candidateNickname[0] == '#' || candidateNickname[0] == ':')
        return NUMERIC::INVALID_NICK;
    
    Client *foundDuplicate = clientHandler.getClientByNick(candidateNickname);
	Client *requestClient = EventHandler::getInstance().getRequestClient();
    if(foundDuplicate){
        if (foundDuplicate == requestClient)
			return NUMERIC::NOTHING;
		else
			return NUMERIC::DUP_NICK;
            //433 "<client> <nick> :Nickname is already in use" (ERR_NICKNAMEINUSE)  
    }
	
    if(candidateNickname.length() > CONFIG::NICKLEN)
        return NUMERIC::INVALID_NICK;

    requestClient->setNickName(candidateNickname);
    return NUMERIC::SUCCESS;
}

NUMERIC::CODE CommandHandler::join(std::vector<std::string>& parameters){
    bool alreadyExists = false;
    bool isValid = true;
    std::queue<std::string> channelList;
    std::queue<std::string> keyList;
    ChannelManager &channelManager = ChannelManager::getInstance();
	
	if(parameters.size() != 2)
		return NUMERIC::NO_PARAM;

	parseByDelimeter(',', parameters[0], channelList);
	parseByDelimeter(',', parameters[1], keyList);
    
    while (channelList.empty() == false){
        std::string channelName = channelList.front();
        if(channelName[0] != '#')
            isValid = false;
        Channel *foundChannel = channelManager.getChannelByName(channelName); 
        if (!foundChannel)
		{
			// 만들기
		}
		else
		{
			if(GET_PERMISSION_K(foundChannel->getPermissions()))
			{
				if(keyList.empty() == true){
					//need password error
				}
				// validChannelCheck(foundChannel, keyList.front());
				keyList.pop();
			} 
		}
        
        // checkValid(string channelName, string UserName, string key?)
        // return NUMERIC::CODE ??
        //
    
        
        
        //채널 존재? key option 이면 keyList 불러와서 확인
        //i option이면 안됨
        // 여기서 다 하죠? get 으로 받아만 와서
       //
        // 토픽 출력
        // 유저 리스트 출력
        //getChannel By ChannelName



        //좋아요 그러면은 다 하고 내부 다른 함수 호출 시에는 'key 같이 주면 key 확인하고 okay! 유저리스트 추가 ' 여기까지 시키는 거 어때요?
        
        //내부 다른 함수 : ChannelManager.joinChannel(Channel*, password) 이런 식
        // 지금 함수에서 ChannelManager.getChannel로 가져와서 예외처리 싹다 하는 거 어때요?

        //Channel의 userlist 추가하는 것도 포함해서용? 아 아니면 패스워드 확인까징? 그러면 딱 넣는 것만 외부 함수 호출하게 만들면 되는거죠?
        // ChannelManager.getChannel 요곳도요 
        //패스워드 확인, 이미 들어와있는지 확인, 존재하는지 확인 등등?

        //넵 
    }

	return NUMERIC::SUCCESS;
}

NUMERIC::CODE CommandHandler::kick(std::vector<std::string>& parameters){
	
	return NUMERIC::SUCCESS;
}

NUMERIC::CODE CommandHandler::invite(std::vector<std::string>& parameters){
	
	return NUMERIC::SUCCESS;
}

NUMERIC::CODE CommandHandler::topic(std::vector<std::string>& parameters){
	
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
    EventHandler& eventHandler = EventHandler::getInstance();
    ChannelManager& channelManager = ChannelManager::getInstance();
    Channel *requestChannel;
    Client *requestClient;
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
    
    eventHandler.setRequestChannel(channelManager.getChannelByName(channelName));
    requestChannel = eventHandler.getRequestChannel();
    requestClient = eventHandler.getRequestClient();

    
    
    if (requestChannel == NULL){
        std::cout << "No SUCH CHAN";
        return NUMERIC::NO_SUCH_CHAN;
    }

    for(int i = 2; i < parameters.size(); i++)
        params.push(parameters[i]);

    if(requestChannel->getOperatorByNick(requestClient->getNickName()) == NULL)
		return NUMERIC::NOT_OPER;
		//error: 482 two #c :You must have channel op access or above to set channel mode k
        // "<client> <channel> :You're not channel operator"

        //check
    std::cout << requestClient->getNickName();
    std::string str;
    for(int i  = 0; i < params.size();i++){
       str = params.front();
       std::cout << str << "\n";
       params.pop();
       params.push(str);       
    }
    

    std::cout << "\n";
    

    fsm.executeMode(params, options);
	// 반환된 에러 처리
    
    return NUMERIC::SUCCESS;
}

void CommandHandler::parseByDelimeter(char delimeter, std::string& parsingLine, std::queue<std::string> &buffer){
    std::stringstream ssLine(parsingLine);
    std::string word;

    while (std::getline(ssLine, word, delimeter))
        buffer.push(word);
}

