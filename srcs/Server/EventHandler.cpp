#include "EventHandler.hpp"
#include "MessageHandler.hpp"

EventHandler::EventHandler()
	: _requestClient(NULL)
	, _changeList()
{
}

EventHandler& EventHandler::getInstance(){
	static EventHandler instance;
	return instance;
}

void EventHandler::changeEvents(uintptr_t ident, int16_t filter,
		uint16_t flags, uint32_t fflags, intptr_t data, void *udata){
	struct kevent temp_event;

	EV_SET(&temp_event, ident, filter, flags, fflags, data, udata);
	_changeList.push_back(temp_event);
}

void EventHandler::init(){
	_serverSocket = SocketHandler::getInstance().getServerSocket();
	_clientManager = &ClientManager::getInstance();
	_kq = kqueue();
	if (_kq == -1)
		throw ErrorHandler::KqueueException();

	changeEvents(_serverSocket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
}

void EventHandler::listenToClients(){
	init();

	while (true){
		_numberOfNewEvents = kevent(_kq, &_changeList[0], _changeList.size(), _event_list, EVENT_BUFFER_SIZE, NULL);
		if (_numberOfNewEvents == -1)
			throw ErrorHandler::KeventException();

		_changeList.clear(); 
		for (int i = 0; i < _numberOfNewEvents; ++i){
			_curEvent = &_event_list[i];

			if (_curEvent->flags & EV_ERROR || _curEvent->flags & EV_EOF){
				if (static_cast<int>(_curEvent->ident) == _serverSocket)
					throw ErrorHandler::KeventException();
				else
					disconnectCurClient();
			}
			else if (_curEvent->filter == EVFILT_READ){
				if (static_cast<int>(_curEvent->ident) == _serverSocket)
					acceptNewClient();
				else
					transportData();
			}
			else if (_curEvent->filter == EVFILT_WRITE){
                Client* curClient = _clientManager->getClientByFD(_curEvent->filter);
                if (!curClient->getSendQue().empty())
                    MessageHandler::getInstance().sendRemainBuffer(curClient);
            }
        }
		//system("leaks ircserv");
	}
}

void EventHandler::disconnectCurClient(){
	ClientManager &clientManager = ClientManager::getInstance();
	Client* curClient = clientManager.getClientByFD(_curEvent->ident);
	
	ChannelManager::getInstance().eraseClientAllChannels(curClient->getNickName());
	clientManager.eraseClientByNick(curClient->getNickName());
	clientManager.eraseClientByFD(_curEvent->ident);
	close(_curEvent->ident);
	MessageHandler::getInstance().flushOutput();
}

void EventHandler::acceptNewClient(){
	int clientSocket;
					
	if ((clientSocket = accept(_serverSocket, NULL, NULL)) == -1)
		return;
	
	if (fcntl(clientSocket, F_SETFL, O_NONBLOCK) == -1)
		throw ErrorHandler::FcntlException();
	changeEvents(clientSocket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
	_clientManager->insertClientByFD(clientSocket);
}

void EventHandler::transportData(){
	Parser &parser = Parser::getInstance();

	char buf[READ_BUFFER_SIZE];
	int n = recv(_curEvent->ident, buf, READ_BUFFER_SIZE, MSG_DONTWAIT);
	this->_requestClient = _clientManager->getClientByFD(_curEvent->ident); 
	
	if (n == -1){
		disconnectCurClient();
		return;
	}
	buf[n] = '\0';
	try{
		//std::cout << "recv : " << buf << "\n";
		parser.parseCommandsAndExecute(buf);
	}
	catch(const std::exception& e){
		std::cerr << e.what() << '\n';
		disconnectCurClient();
	}
	//debugPrint();
}    

Client* EventHandler::getRequestClient() const
{
	return _requestClient;
}

Channel* EventHandler::getRequestChannel()const
{
	return this->_requestChannel;
}

void EventHandler::setRequestChannel(Channel* channel)
{
	_requestChannel = channel;
}

void debugPrint(){
	std::cout << "---- TEST -----\n";
	ClientManager& clm = ClientManager::getInstance();
	ChannelManager& chm = ChannelManager::getInstance();

	clm.printClients();
	chm.printChannels();
	std::cout << "\n";
}