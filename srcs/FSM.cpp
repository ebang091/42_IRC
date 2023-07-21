#include "FSM.hpp"

FSM::FSM()
	: _eventHandler(NULL)
	, _channel(NULL)
	, _client(NULL)
{		
}

FSM& FSM::getInstance(){
	static FSM instance;
	return instance;
}

NUMERIC::CODE FSM::executeAndChangeState(STATE::CODE &state, std::queue<std::string>& params, char c) {
    INPUT::CODE inputCode = getInput(c);
	
    if (inputCode == INPUT::NONE)
        return NUMERIC::INVALID_MODE;
        //"<client> <modechar> :is unknown mode char to me" ERR_UNKNOWNMODE (472) 
    
    typedef NUMERIC::CODE (FSM::*FP)(std::queue<std::string>&, STATE::CODE&);
    FP fp[STATE::SIZE][INPUT::SIZE] = {{&FSM::plusN, &FSM::plusT, &FSM::plusI, &FSM::plusK, &FSM::plusO, &FSM::plusL, &FSM::toPlus, &FSM::toMinus}
                                    , {&FSM::minusN, &FSM::minusT, &FSM::minusI, &FSM::minusK, &FSM::minusO, &FSM::minusL, &FSM::toPlus, &FSM::toMinus}
                                    , {NULL, NULL, NULL, NULL, NULL, NULL, &FSM::toPlus, &FSM::toMinus}
                                    };
    
	NUMERIC::CODE errCode = (this->*fp[state][inputCode])(params, state);

	return errCode;
}

void FSM::executeMode(std::queue<std::string>& params, const std::string& options)
{
	_eventHandler = &EventHandler::getInstance();
	_channel = _eventHandler->getRequestChannel();
    _client = _eventHandler->getRequestClient();
	STATE::CODE state = STATE::NONE;

	// channelInfo message

    for(int i = 0; i < options.size(); i++){
        NUMERIC::CODE result = executeAndChangeState(state, params, options[i]); 
		// mode message
		// broadcast
	}
}

INPUT::CODE FSM::getInput(char c) const{
    if(c == '+')
        return INPUT::PLUS;
    else if(c == '-')
        return INPUT::MINUS;
    else if(c == 'n')
        return INPUT::N;
    else if(c == 'i')
        return INPUT::I;
    else if(c == 'k')
        return INPUT::K;
    else if(c == 'o')
        return INPUT::O;
    else if(c == 'l')
        return INPUT::L;
	else if(c == 't')
        return INPUT::T;
    else
        return INPUT::NONE;
}

#include <iostream>
NUMERIC::CODE FSM::plusN(std::queue<std::string>& params, STATE::CODE& state){
	std::cout << "+N called\n";
	
	return NUMERIC::NOTHING;
}

NUMERIC::CODE FSM::plusT(std::queue<std::string>& params, STATE::CODE& state){
	std::cout << "+T called\n";

	std::string broadcast;
    char permissions = _channel->getPermissions();
	if (GET_PERMISSION_T(permissions))
		return NUMERIC::NOTHING;

    _channel->setPermission(SWITCH_PERMISSION_T(permissions));

	// USER root root 127.0.0.1 :root
	broadcast = "";
    broadcast += _client->getNickName();
	broadcast += "!";
	broadcast += _client->getUserName();
	broadcast += "@";
	broadcast += _client->getIpAddress();
	broadcast += " ";
	
	broadcast += "MODE ";
    broadcast += "#";
    broadcast += _channel->getName();
    broadcast += " :+t";
	broadcast += "\n";// 개행 여부는 아직 잘 모르겠음
    
    //channelManager.speakToAllClients(_channel, broadcast);
	return NUMERIC::SUCCESS;
}

NUMERIC::CODE FSM::plusI(std::queue<std::string>& params, STATE::CODE& state){
	std::cout << "+I called\n";

	std::string broadcast;
    char permissions = _channel->getPermissions();
	if (GET_PERMISSION_I(permissions))
		return NUMERIC::NOTHING;

    _channel->setPermission(SWITCH_PERMISSION_I(permissions));

	// USER root root 127.0.0.1 :root
	broadcast = "";
    broadcast += _client->getNickName();
	broadcast += "!";
	broadcast += _client->getUserName();
	broadcast += "@";
	broadcast += _client->getIpAddress();
	broadcast += " ";
	
	broadcast += "MODE ";
    broadcast += "#";
    broadcast += _channel->getName();
    broadcast += " :+i ";
	broadcast += "\n";// 개행 여부는 아직 잘 모르겠음
    
    //channelManager.speakToAllClients(_channel, broadcast);
	return NUMERIC::SUCCESS;
}

NUMERIC::CODE FSM::plusK(std::queue<std::string>& params, STATE::CODE& state){
    std::cout << "+K called\n";
    std::string password;
    std::string broadcast;
    
    if(params.empty())
		return NUMERIC::NO_PARAM;
        //error: 696 ERR_INCALIDMODEPARAM "<client> <target chan/user> <mode char> <parameter> :<description>"
    
    password = params.front();
    params.pop();

	char permissions = _channel->getPermissions();
	if (GET_PERMISSION_K(permissions))
		return NUMERIC::NOTHING;

    _channel->setPermission(SWITCH_PERMISSION_K(permissions));   
	_channel->setPassword(password);

	// USER root root 127.0.0.1 :root
	broadcast = "";
    broadcast += _client->getNickName();
	broadcast += "!";
	broadcast += _client->getUserName();
	broadcast += "@";
	broadcast += _client->getIpAddress();
	broadcast += " ";
	
	broadcast += "MODE ";
    broadcast += "#";
    broadcast += _channel->getName();
    broadcast += " +k ";
    broadcast += password;
	broadcast += "\n";// 개행 여부는 아직 잘 모르겠음
    
    //channelManager.speakToAllClients(_channel, broadcast);
	return NUMERIC::SUCCESS;
}

NUMERIC::CODE FSM::plusO(std::queue<std::string>& params, STATE::CODE& state){
	std::cout << "+O called\n";
	std::string targetName;
	std::string broadcast;

	//param이 없으면 696
	if(params.size() == 0)
		return NUMERIC::NO_PARAM;
	
	targetName = params.front();
	params.pop();
	
	Client *find = _channel->getClientByNick(targetName);
	//user 가 channel에 없으면 401 no such nick
	if(find == NULL)
		return NUMERIC::NO_SUCH_NICK;
	
	//user가 이미 channel operator 이면 무시
	if(find == _channel->getOperatorByNick(targetName))
		return NUMERIC::NOTHING;

	//channel의 operator list에 param 인 user 추가
	_channel->insertOperator(find);
	
	// one!root@127.0.0.1 MODE #c +o :two

	broadcast = "";
	broadcast += _client->getNickName();
	broadcast += "!";
	broadcast += _client->getUserName();
	broadcast += "@";
	broadcast += _client->getIpAddress();
	broadcast += " ";
	
	broadcast += "MODE ";
    broadcast += "#";
    broadcast += _channel->getName();
    broadcast += " +o :";
    broadcast += targetName;
	broadcast += "\n";// 개행 여부는 아직 잘 모르겠음
	
	//channelManager.speakToAllClients(_channel, broadcast);
	return NUMERIC::SUCCESS;
}

NUMERIC::CODE FSM::plusL(std::queue<std::string>& params, STATE::CODE& state){
	std::cout << "+L called\n";
	
	std::string limitStr;
    std::string broadcast;
	
	long limitNum;
    
    if(params.empty())
		return NUMERIC::NO_PARAM;
        //error: 696 ERR_INCALIDMODEPARAM "<client> <target chan/user> <mode char> <parameter> :<description>"
    
    limitStr = params.front();
    params.pop();   

	std::istringstream iss(limitStr);
	iss >> limitNum;

	if (limitNum < 0)
		return NUMERIC::UNKNOWN_ERR; //Invalid limitStr mode parameter. Syntax: <limitStr>.)
	// overflow, not a number
	if (iss.fail())
		limitNum = 0;

	std::stringstream ss;
	ss << limitNum;
	limitStr = ss.str();
	
	char permissions = _channel->getPermissions();
    _channel->setPermission(SWITCH_PERMISSION_L(permissions));	
	_channel->setLimit(limitNum);

	// USER root root 127.0.0.1 :root
	broadcast = "";
    broadcast += _client->getNickName();
	broadcast += "!";
	broadcast += _client->getUserName();
	broadcast += "@";
	broadcast += _client->getIpAddress();
	broadcast += " ";
	
	broadcast += "MODE ";
    broadcast += "#";
    broadcast += _channel->getName();
    broadcast += " +l ";
    broadcast += ss.str();
	broadcast += "\n";// 개행 여부는 아직 잘 모르겠음
    
    //channelManager.speakToAllClients(_channel, broadcast);
	return NUMERIC::SUCCESS;
}

NUMERIC::CODE FSM::minusN(std::queue<std::string>& params, STATE::CODE& state){
	std::cout << "-N called\n";
	return NUMERIC::UNKNOWN_ERR; //"you cannot unset option n"
}

NUMERIC::CODE FSM::minusT(std::queue<std::string>& params, STATE::CODE& state){
	std::cout << "-T called\n";

	std::string broadcast;
    char permissions = _channel->getPermissions();
	if (!(GET_PERMISSION_T(permissions)))
		return NUMERIC::NOTHING;

    _channel->setPermission(SWITCH_PERMISSION_T(permissions));

	// USER root root 127.0.0.1 :root
	broadcast = "";
    broadcast += _client->getNickName();
	broadcast += "!";
	broadcast += _client->getUserName();
	broadcast += "@";
	broadcast += _client->getIpAddress();
	broadcast += " ";
	
	broadcast += "MODE ";
    broadcast += "#";
    broadcast += _channel->getName();
    broadcast += " :-t ";
	broadcast += "\n";// 개행 여부는 아직 잘 모르겠음
	return NUMERIC::SUCCESS;
}

NUMERIC::CODE FSM::minusI(std::queue<std::string>& params, STATE::CODE& state){
	std::string broadcast;
    char permissions = _channel->getPermissions();
	if (!(GET_PERMISSION_I(permissions)))
		return NUMERIC::NOTHING;

    _channel->setPermission(SWITCH_PERMISSION_I(permissions));

	// USER root root 127.0.0.1 :root
	broadcast = "";
    broadcast += _client->getNickName();
	broadcast += "!";
	broadcast += _client->getUserName();
	broadcast += "@";
	broadcast += _client->getIpAddress();
	broadcast += " ";
	
	broadcast += "MODE ";
    broadcast += "#";
    broadcast += _channel->getName();
    broadcast += " :+i ";
	broadcast += "\n";// 개행 여부는 아직 잘 모르겠음
    
    //channelManager.speakToAllClients(_channel, broadcast);
	return NUMERIC::SUCCESS;
}

NUMERIC::CODE FSM::minusK(std::queue<std::string>& params, STATE::CODE& state){
	std::cout << "-K called\n";
	std::string password;
    std::string broadcast;

	char permissions = _channel->getPermissions();
	if (!(GET_PERMISSION_K(permissions)))
		return NUMERIC::UNKNOWN_ERR; // 400 key already unset.
    
	if (params.empty())
		return NUMERIC::NO_PARAM;	// 696

	password = params.front();
    params.pop();
	if (password != _channel->getPassword())
		return NUMERIC::KEY_ALREADY_SET;	// 497?

    _channel->setPermission(SWITCH_PERMISSION_K(permissions));   
	_channel->setPassword("");

	// USER root root 127.0.0.1 :root
	broadcast = "";
    broadcast += _client->getNickName();
	broadcast += "!";
	broadcast += _client->getUserName();
	broadcast += "@";
	broadcast += _client->getIpAddress();
	broadcast += " ";
	
	broadcast += "MODE ";
    broadcast += "#";
    broadcast += _channel->getName();
    broadcast += " -k ";
    broadcast += password;
	broadcast += "\n";// 개행 여부는 아직 잘 모르겠음
    
    //channelManager.speakToAllClients(_channel, broadcast);
	return NUMERIC::SUCCESS;
}

NUMERIC::CODE FSM::minusO(std::queue<std::string>& params, STATE::CODE& state){
	std::cout << "-O called\n";

	std::string targetName;
	std::string broadcast;

	//param이 없으면 696
	if(params.size() == 0)
		return NUMERIC::NO_PARAM;
	
	targetName = params.front();
	params.pop();
	
	Client *find = _channel->getClientByNick(targetName);
	//user 가 channel에 없으면 401 no such nick
	if(find == NULL)
		return NUMERIC::NO_SUCH_NICK;
	
	//user가 이미 channel operator가 아니면 무시??
	if(find != _channel->getOperatorByNick(targetName))
		return NUMERIC::NOTHING;

	//channel의 operator list에 param 인 user 제거
	_channel->eraseOperator(find->getNickName());
	
	// one!root@127.0.0.1 MODE #c +o :two
	broadcast = "";
	broadcast += _client->getNickName();
	broadcast += "!";
	broadcast += _client->getUserName();
	broadcast += "@";
	broadcast += _client->getIpAddress();
	broadcast += " ";
	
	broadcast += "MODE ";
    broadcast += "#";
    broadcast += _channel->getName();
    broadcast += " -o :";
    broadcast += targetName;
	broadcast += "\n";// 개행 여부는 아직 잘 모르겠음
	
	//channelManager.speakToAllClients(_channel, broadcast);
	return NUMERIC::SUCCESS;
}

NUMERIC::CODE FSM::minusL(std::queue<std::string>& params, STATE::CODE& state){
	std::cout << "-L called\n";
	std::string broadcast;

	char permissions = _channel->getPermissions();
	if(!(GET_PERMISSION_L(permissions)))
		return NUMERIC::NOTHING;
    _channel->setPermission(SWITCH_PERMISSION_L(permissions));
	_channel->setLimit(CONFIG::USERLIMIT);

	// USER root root 127.0.0.1 :root
	broadcast = "";
    broadcast += _client->getNickName();
	broadcast += "!";
	broadcast += _client->getUserName();
	broadcast += "@";
	broadcast += _client->getIpAddress();
	broadcast += " ";
	
	broadcast += "MODE ";
    broadcast += "#";
    broadcast += _channel->getName();
    broadcast += " -l";
    broadcast += "\n";// 개행 여부는 아직 잘 모르겠음
	//limit
	return NUMERIC::SUCCESS;
}

NUMERIC::CODE FSM::toPlus(std::queue<std::string>& params, STATE::CODE& state){
	std::cout << "++ called\n";
    state = STATE::PLUS;
	return NUMERIC::SUCCESS;
}

NUMERIC::CODE FSM::toMinus(std::queue<std::string>& params, STATE::CODE& state){
	std::cout << "-- called\n";
    state = STATE::MINUS;
	return NUMERIC::SUCCESS;
}
