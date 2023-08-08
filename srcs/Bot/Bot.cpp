#include "CommandHandler.hpp"
#include "ClientManager.hpp"
#include "Channel.hpp"
#include "MessageHandler.hpp"
#include "ChannelManager.hpp"
#include "Bot.hpp"

Bot::Bot()
	: _name(BOT_NAME)
{
}

Bot& Bot::getInstance() {
	static Bot instance;
	return instance;
}

const std::string& Bot::getName() const{
    return this->_name;
}

std::string Bot::getSendingMessage(BOT::CODE code){
    switch(code){
    case(BOT::GET_NOTICE):
        return "***NOTICE*** " + _requestChannel->getNotice();
    case(BOT::GET_WELCOME):
        return "***WELCOME*** " +_requestChannel->getWelcomeMsg();
    case(BOT::SET_NOTICE):
    	if (!_requestChannel->getOperatorByNick(_requestClient->getNickName())){
            _messageHandler->BotSetDescriptionByCode(NUMERIC::NOT_OPER);
		    _messageHandler->sendPrivMsgToUser(_requestClient);
            return "";
        }
        else{
            _requestChannel->setNotice(_description);
		    return "Successfully set notice to '" + _description + "'.";
		}
    case(BOT::SET_WELCOME):
        if (!_requestChannel->getOperatorByNick(_requestClient->getNickName())){
            _messageHandler->BotSetDescriptionByCode(NUMERIC::NOT_OPER);
		    _messageHandler->sendPrivMsgToUser(_requestClient);
            return "";
        }
        else{
    		_requestChannel->setWelcomeMsg(_description);
        	return "Successfully set welcome message to '" + _description + "'.";
        }
	default:
		return "";
    }
}

BOT::CODE Bot::parseMessage(std::vector<std::string>& parameters){
	std::string fState[5] = {"", "GET", "SET", "NOTICE", "WELCOME"};

    int state = 1;
    int which = 1;

    std::string channelName = parameters[1];
    _messageHandler->setTargetName(_requestClient->getNickName());
    channelName.erase(0,1);
    if(channelName.empty() || channelName.front() != CHANNEL_PREFIX){
        _messageHandler->BotSetDescriptionByCode(NUMERIC::BAD_CHAN_MASK);
		_messageHandler->sendPrivMsgToUser(_requestClient);
        return BOT::FAIL;
    }
	channelName.erase(0, 1);

	_requestChannel = _channelManager->getChannelByName(channelName);
	if (!_requestChannel){
        _messageHandler->BotSetDescriptionByCode(NUMERIC::NO_SUCH_CHAN);
		_messageHandler->sendPrivMsgToUser(_requestClient);
		return BOT::FAIL;
	}
        
	if (!_requestChannel->getClientByNick(_requestClient->getNickName())){
		_messageHandler->BotSetDescriptionByCode(NUMERIC::NOT_ON_CHAN);
		_messageHandler->sendPrivMsgToUser(_requestClient);
		return BOT::FAIL;
	}

    for(int i = 1; i < 5; i++){
        if(strcasecmp(parameters[2].c_str(), fState[i].c_str()) == 0)
            state += i;
        if(strcasecmp(parameters[3].c_str(), fState[i].c_str()) == 0)
            which += i;
    }
    if (state * which < 8)
        return BOT::CMD_NOT_FOUND;

	_description = "";
    for (size_t i = 4; i < parameters.size(); ++i){
		_description += parameters[i];
		if (i != parameters.size() - 1)
            _description += " ";
	}
    return static_cast<BOT::CODE>(state * which);
}

void Bot::sendMessage(std::vector<std::string>& parameters, Client* requestClient){
    _channelManager = &ChannelManager::getInstance();
	_messageHandler = &MessageHandler::getInstance();
	_requestClient = requestClient;

    Client temp(BOT_NAME, BOT_NAME, "127.0.0.1");
    _messageHandler->setRequestClient(&temp);
    _messageHandler->setTargetName(_requestClient->getNickName());

	if(parameters.size() < 4){
		_messageHandler->setDescription("🍫 usage: /msg ebangBot <#targetChannel> <GET/SET> <NOTICE/WELCOME> <message you want to set>.");
		_messageHandler->sendPrivMsgToUser(_requestClient);
		return;
	}

	BOT::CODE code = parseMessage(parameters);
	std::string message = getSendingMessage(code);

	if(message.empty())
		return;

    if(code == BOT::CMD_NOT_FOUND)
		_messageHandler->setDescription("🍫 usage: /msg ebangBot <#targetChannel> <GET/SET> <NOTICE/WELCOME> <message you want to set>.");
	else
		_messageHandler->setDescription("🍫 " + message);
	
    std::set<int> isSet;
    _messageHandler->sendPrivMsgToChannel(isSet);
	_messageHandler->setRequestClient(_requestClient);
}

void Bot::sendWelcomeMessage(std::string channelName, Client* requestClient){
	_messageHandler = &MessageHandler::getInstance();
    _channelManager = &ChannelManager::getInstance();
    channelName.erase(0, 1);
    _requestChannel = _channelManager->getChannelByName(channelName);
	_messageHandler->setCommand("PRIVMSG");
    _messageHandler->setDescription("🍫 Welcome " + requestClient->getNickName() + "!! [" + _requestChannel->getWelcomeMsg() + "], *** Notice *** [" + _requestChannel->getNotice() + "]" );
    
	Client bot(BOT_NAME, BOT_NAME, "127.0.0.1");
    _messageHandler->setRequestClient(&bot);
   
    std::set<int> isSet;
    _messageHandler->sendPrivMsgToChannel(isSet);
	_messageHandler->setRequestClient(requestClient);
	_messageHandler->setCommand("JOIN");
}