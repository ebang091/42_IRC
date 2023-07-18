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
}

ERROR::CODE CommandHandler::quit(std::vector<std::string>& parameters){

}

ERROR::CODE CommandHandler::nick(std::vector<std::string>& parameters){
    ClientHandler &clientHandler = ClientHandler::getInstance();

    bool isValid = true;
    if(parameters.size() == 0)
        return ERROR::NO_PARAM;
    if(parameters.size() != 1)
		return ERROR::INVALID_NICK; // 
        //  432 "<client> <nick> :Erroneus nickname" 
    std::string candidateNickname = parameters[0];
    if(candidateNickname[0] == '#' || candidateNickname[0] == ':')
        isValid =  false;
    
    
    //NICK nick name
    
    



}

ERROR::CODE CommandHandler::join(std::vector<std::string>& parameters){

}

ERROR::CODE CommandHandler::kick(std::vector<std::string>& parameters){

}

ERROR::CODE CommandHandler::invite(std::vector<std::string>& parameters){

}

ERROR::CODE CommandHandler::topic(std::vector<std::string>& parameters){

}

ERROR::CODE CommandHandler::mode(std::vector<std::string>& parameters){

}

