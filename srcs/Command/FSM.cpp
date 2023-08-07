#include "FSM.hpp"
#include "ClientManager.hpp"

FSM::FSM()
	: _channel(NULL)
	, _client(NULL)
	, _clientManager(NULL)
	, _eventHandler(NULL)
	, _messageHandler(NULL)
{		
}

FSM& FSM::getInstance(){
	static FSM instance;
	return instance;
}

void FSM::setMessageHandler(MessageHandler *messagehandler){
	this->_messageHandler = messagehandler;
}

void FSM::executeAndChangeState(STATE::CODE &state, std::queue<std::string>& params, char c) {
    INPUT::CODE inputCode = getInput(c);
	_messageHandler->setChannel(_channel->getName());
	_messageHandler->setState(state);
	_messageHandler->setOption(c);
	
    if (inputCode == INPUT::NONE)
        return _messageHandler->sendInvalidModeError(NUMERIC::INVALID_MODE);
    
    typedef void (FSM::*FP)(std::queue<std::string>&, STATE::CODE&);
    FP fp[STATE::SIZE][INPUT::SIZE] = {{&FSM::plusN, &FSM::plusT, &FSM::plusI, &FSM::plusK, &FSM::plusO, &FSM::plusL, &FSM::toPlus, &FSM::toMinus}
                                    , {&FSM::minusN, &FSM::minusT, &FSM::minusI, &FSM::minusK, &FSM::minusO, &FSM::minusL, &FSM::toPlus, &FSM::toMinus}
                                    , {NULL, NULL, NULL, NULL, NULL, NULL, &FSM::toPlus, &FSM::toMinus}
                                    };
    if (fp[state][inputCode] != NULL)
		(this->*fp[state][inputCode])(params, state);
}

void FSM::executeMode(std::queue<std::string>& params, const std::string& options){
	_eventHandler = &EventHandler::getInstance();
	_clientManager = &ClientManager::getInstance();
	_channel = _eventHandler->getRequestChannel();
    _client = _eventHandler->getRequestClient();
	STATE::CODE state = STATE::NONE;

    for(size_t i = 0; i < options.size(); i++)
		executeAndChangeState(state, params, options[i]);
}

INPUT::CODE FSM::getInput(char c) const{
	switch (c)
	{
	case '+':
		return INPUT::PLUS;
	case '-':
		return INPUT::MINUS;
	case 'n':
		return INPUT::N;
	case 'i':
		return INPUT::I;
	case 'k':
		return INPUT::K;
	case 'o':
		return INPUT::O;
	case 'l':
		return INPUT::L;
	case 't':
		return INPUT::T;
	default:
		return INPUT::NONE;
	}
}

void FSM::plusN(std::queue<std::string>& params, STATE::CODE& state){
	(void)state;
	(void)params;
}

void FSM::plusT(std::queue<std::string>& params, STATE::CODE& state){
	(void)state;
	(void)params;
	std::string broadcast;
    char permissions = _channel->getPermissions();
	if (GET_PERMISSION_T(permissions))
		return;

	_messageHandler->setDescription("");
    _channel->setPermission(SWITCH_PERMISSION_T(permissions));
	_messageHandler->sendModeSuccess();
}

void FSM::plusI(std::queue<std::string>& params, STATE::CODE& state){
	std::string broadcast;
	(void)state;
	(void)params;

    char permissions = _channel->getPermissions();
	if (GET_PERMISSION_I(permissions))
		return;

	_channel->setPermission(SWITCH_PERMISSION_I(permissions));

	_messageHandler->setDescription("");
	_messageHandler->sendModeSuccess();
}

void FSM::plusK(std::queue<std::string>& params, STATE::CODE& state){
    std::string password;
    std::string broadcast;
	(void)state;

    if(params.empty())
		return _messageHandler->sendErrorNoModeParam("");
    password = params.front();
	if(password.size() > CAP::KEYLEN)
		password = password.substr(0, CAP::KEYLEN);
    params.pop();

	char permissions = _channel->getPermissions();
	if (GET_PERMISSION_K(permissions))
		return;

    _channel->setPermission(SWITCH_PERMISSION_K(permissions));   
	_channel->setPassword(password);
	_messageHandler->setDescription(password);
	_messageHandler->sendModeSuccess();
}

void FSM::plusO(std::queue<std::string>& params, STATE::CODE& state){
	std::string targetName;
	std::string broadcast;
	(void)state;

	if(params.empty())
		return _messageHandler->sendErrorNoModeParam("");
	
	targetName = params.front();
	params.pop();
	_messageHandler->setTargetName(targetName);
	
	if(!_clientManager->getClientByNick(targetName))
		return _messageHandler->sendErrorWithNickAndTargetName(NUMERIC::NO_SUCH_NICK);
	
	Client *find = _channel->getClientByNick(targetName);

	if(!find || find == _channel->getOperatorByNick(targetName))
		return;

	_channel->insertOperator(find);
	_messageHandler->setDescription(targetName);
	_messageHandler->sendModeSuccess();
}

void FSM::plusL(std::queue<std::string>& params, STATE::CODE& state){
	std::string limitStr;
    std::string broadcast;
	(void)state;
	
	long limitNum;
    
    if(params.empty())
		return _messageHandler->sendErrorNoModeParam("");
    
    limitStr = params.front();
    params.pop();   

	std::istringstream iss(limitStr);
	iss >> limitNum;

	if (limitNum < 0)
		return _messageHandler->sendErrorNoModeParam(limitStr + " :Invalid limit mode parameter. Syntax: <limit>.");

	if (iss.fail())
		limitNum = 0;

	std::stringstream ss;
	ss << limitNum;
	limitStr = ss.str();
	
	char permissions = _channel->getPermissions();
	if (!(GET_PERMISSION_L(permissions)))
		_channel->setPermission(SWITCH_PERMISSION_L(permissions));
	if (_channel->getLimit() == limitNum)
		return;

	_channel->setLimit(limitNum);
	_messageHandler->setDescription(ss.str());
	_messageHandler->sendModeSuccess();
}

void FSM::minusN(std::queue<std::string>& params, STATE::CODE& state){
	(void)state;
	(void)params;

	return _messageHandler->sendErrorUnknown(":You cannot unset option n");
}

void FSM::minusT(std::queue<std::string>& params, STATE::CODE& state){
	(void)state;
	(void)params;
	std::string broadcast;

    char permissions = _channel->getPermissions();
	if (!(GET_PERMISSION_T(permissions)))
		return;

    _channel->setPermission(SWITCH_PERMISSION_T(permissions));
	_messageHandler->setDescription("");
	_messageHandler->sendModeSuccess();
}

void FSM::minusI(std::queue<std::string>& params, STATE::CODE& state){
	std::string broadcast;
	(void)state;
	(void)params;

    char permissions = _channel->getPermissions();
	if (!(GET_PERMISSION_I(permissions)))
		return;

    _channel->setPermission(SWITCH_PERMISSION_I(permissions));
	_messageHandler->setDescription("");
	_messageHandler->sendModeSuccess();
}

void FSM::minusK(std::queue<std::string>& params, STATE::CODE& state){
	std::string password;
    std::string broadcast;
	(void)state;

	if (params.empty())
		return _messageHandler->sendErrorNoModeParam("");

	password = params.front();
    params.pop();

	char permissions = _channel->getPermissions();
	if (!(GET_PERMISSION_K(permissions)))
		return;
	
	if (password != _channel->getPassword())
		return _messageHandler->sendErrorWithChannel(NUMERIC::KEY_ALREADY_SET);

    _channel->setPermission(SWITCH_PERMISSION_K(permissions));   
	_channel->setPassword("");
	_messageHandler->setDescription(password);
	_messageHandler->sendModeSuccess();
}

void FSM::minusO(std::queue<std::string>& params, STATE::CODE& state){
	(void)state;
	std::string targetName;
	std::string broadcast;

	if(params.empty())
		return _messageHandler->sendErrorNoModeParam("");
	
	targetName = params.front();
	params.pop();
	
	Client *find = _channel->getClientByNick(targetName);
	if(find == NULL)
		return _messageHandler->sendErrorWithNickAndTargetName(NUMERIC::NO_SUCH_NICK);
	
	if(find != _channel->getOperatorByNick(targetName))
		return;

	_channel->eraseOperator(find->getNickName());
	_messageHandler->setDescription(targetName);
	_messageHandler->sendModeSuccess();
}

void FSM::minusL(std::queue<std::string>& params, STATE::CODE& state){
	(void)state;
	(void)params;
	std::string broadcast;

	char permissions = _channel->getPermissions();
	if(!(GET_PERMISSION_L(permissions)))
		return;

	_messageHandler->setDescription(_messageHandler->ntoStr(_channel->getLimit()));
    _channel->setPermission(SWITCH_PERMISSION_L(permissions));
	_channel->setLimit(CONFIG::USERLIMIT);
	_messageHandler->sendModeSuccess();
}

void FSM::toPlus(std::queue<std::string>& params, STATE::CODE& state){
	(void)params;
    state = STATE::PLUS;
}

void FSM::toMinus(std::queue<std::string>& params, STATE::CODE& state){
	(void)params;
    state = STATE::MINUS;
}
