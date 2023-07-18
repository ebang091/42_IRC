#include "CommandHandler.hpp"

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
    typedef ERROR::CODE (CommandHandler::*FP)(std::vector<std::string>&);
    FP funcs[CMD::SIZE] = {&CommandHandler::cap, &CommandHandler::quit, &CommandHandler::nick, 
			&CommandHandler::join, &CommandHandler::kick, &CommandHandler::invite,
			&CommandHandler::topic, &CommandHandler::mode};
    
	(this->*funcs[static_cast<int>(cmdCode)])(parameters);
}

ERROR::CODE CommandHandler::cap(std::vector<std::string>& parameters){
    if (parameters.size() == 1 && parameters[0] == "LS"){
        //출력 .. 쫘르륵 : Channel, Client 에서 불러와서 define 된 값. 
    }
	return ERROR::SUCCESS;
}

ERROR::CODE CommandHandler::quit(std::vector<std::string>& parameters){
	
	return ERROR::SUCCESS;
}

ERROR::CODE CommandHandler::nick(std::vector<std::string>& parameters){
    ClientManager &clientHandler = ClientManager::getInstance();
    std::string candidateNickname = parameters[0];
    bool isValid = true;

    if(parameters.size() == 0)
        return ERROR::NO_PARAM;
    if(parameters.size() != 1)
		isValid =  false;
        //  432 "<client> <nick> :Erroneus nickname"  (ERR_ERRONEUSNICKNAME )
    if(candidateNickname[0] == '#' || candidateNickname[0] == ':')
        return ERROR::INVALID_NICK;
    
    Client *foundDuplicate = clientHandler.getClientByNick(candidateNickname);
	Client *requestClient = EventHandler::getInstance().getRequestClient();
    if(foundDuplicate){
        if (foundDuplicate == requestClient)
			return ERROR::NOTHING;
		else
			return ERROR::DUP_NICK;
            //433 "<client> <nick> :Nickname is already in use" (ERR_NICKNAMEINUSE)  
    }
	
    if(candidateNickname.length() > CONFIG::NICKLEN)
        return ERROR::INVALID_NICK;

    requestClient->SetNickName(candidateNickname);
    return ERROR::SUCCESS;
}

ERROR::CODE CommandHandler::join(std::vector<std::string>& parameters){
    
    bool alreadyExists = false;
    bool isValid = false;
    std::queue<std::string> channelList;
    std::queue<std::string> keyList;
	
	if(parameters.size() != 2)
		return ERROR::NO_PARAM;
	
    std::string candidateChannelList = parameters[0]; //#channel1,#channel2,...  ','로 구분
    std::string candidateKeyList = parameters[1];// 그뒤에 password가 주어지는 걸로 알고 있습니다.. ','로 구분
	
	std::stringstream ss(candidateChannelList);
	std::string word;
	while (std::getline(ss, word, ','))
	{
		// ,로 split되어 word에 저장
	}
    
	for(int i = 0; i < candidateChannelList.size();i++){
        //#를 기준으로 자를 수 있나염????#다음이 channel명. 
       
    }
	return ERROR::SUCCESS;
}

ERROR::CODE CommandHandler::kick(std::vector<std::string>& parameters){
	
	return ERROR::SUCCESS;
}

ERROR::CODE CommandHandler::invite(std::vector<std::string>& parameters){
	
	return ERROR::SUCCESS;
}

ERROR::CODE CommandHandler::topic(std::vector<std::string>& parameters){
	
	return ERROR::SUCCESS;
}

ERROR::CODE CommandHandler::mode(std::vector<std::string>& parameters){
	
	return ERROR::SUCCESS;
}

