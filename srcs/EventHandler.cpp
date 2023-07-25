#include "EventHandler.hpp"

EventHandler::EventHandler()
	: _requestClient(NULL)
    , _changeList()
{
}

EventHandler& EventHandler::getInstance(){
	static EventHandler instance;
	return instance;
}

void EventHandler::changeEvents(std::vector<struct kevent>& changeList, uintptr_t ident, int16_t filter,
        uint16_t flags, uint32_t fflags, intptr_t data, void *udata){
    struct kevent temp_event;

    EV_SET(&temp_event, ident, filter, flags, fflags, data, udata);
    changeList.push_back(temp_event);
}

void test()
{
    std::cout << "---- TEST -----\n";
    ClientManager& clm = ClientManager::getInstance();
    ChannelManager& chm = ChannelManager::getInstance();

    clm.printClients();
    chm.printChannels();
    std::cout << "\n";
}

void EventHandler::init()
{
	_serverSocket = SocketHandler::getInstance().getServerSocket();
    _kq = kqueue();
    if (_kq == -1)
        throw ErrorHandler::KqueueException();
	
	int option = 1;
	if (setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) == -1)
		throw ErrorHandler::SocketException();

    changeEvents(_changeList, _serverSocket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
}

void EventHandler::listenToClients(){
    Parser &parser = Parser::getInstance();
    ClientManager &clientManager = ClientManager::getInstance();
	init();

    while (1)
    {
        _numberOfNewEvents = kevent(_kq, &_changeList[0], _changeList.size(), _event_list, EVENT_BUFFER_SIZE, NULL);
        if (_numberOfNewEvents == -1)
            throw ErrorHandler::KeventException();

        _changeList.clear(); // clear changeList for new changes

        for (int i = 0; i < _numberOfNewEvents; ++i){
            _curEvent = &_event_list[i];

             /* check error event return */
            if (_curEvent->flags & EV_ERROR)
            {
                if (_curEvent->ident == _serverSocket){
					throw ErrorHandler::KeventException();
                }
                else
                {
                    //어쩌면 quit 일 때도 이럴 수도..?
                    close(_curEvent->ident);
                    clientManager.eraseClientByFD(_curEvent->ident);
                }
            }
			else if (_curEvent->filter == EVFILT_READ)
			{
				if (_curEvent->ident == _serverSocket)
                {
                    /* accept new client */
                    int clientSocket;
                    
                    if ((clientSocket = accept(_serverSocket, NULL, NULL)) == -1)
                        throw ErrorHandler::AcceptException();
                    
                    //client 와 통신해서 비밀번호 입력해야함
                    fcntl(clientSocket, F_SETFL, O_NONBLOCK);

                    /* add event for client socket - add read && write event */
                    changeEvents(_changeList, clientSocket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
                    clientManager.insertClientByFD(clientSocket);
                    
                }
                else
                {
                    /* read data from client */
                    char buf[READ_BUFFER_SIZE];
					int n = recv(_curEvent->ident, buf, READ_BUFFER_SIZE, MSG_DONTWAIT);
                    this->_requestClient = clientManager.getClientByFD(_curEvent->ident); 

                    std::cout << "recv len : " << n << "\n";
					if (n == -1){
                        disconnectCurClient(_curEvent);
						continue;
					}

                	if (n > READ_MAX){
                	    //안된다고 출력하고 아무 반응도 안함.   
                        continue;                 	
                	}

                    buf[n] = '\0';
                    try
                    {
                    	parser.parseCommandsAndExecute(buf);//파싱하고 실행
                    }
                    catch(const std::exception& e)
                    {
                        std::cerr << e.what() << '\n';
						disconnectCurClient(_curEvent);
                    }
                    test();
                    std::cout << "Event\n";
                }
			}
        }
    }
}

void EventHandler::disconnectCurClient(struct kevent* _curEvent)
{
	ClientManager &clientManager = ClientManager::getInstance();
	Client* curClient = clientManager.getClientByFD(_curEvent->ident);
	
	ChannelManager::getInstance().eraseClientAllChannels(curClient->getNickName());
	clientManager.eraseClientByNick(curClient->getNickName());
	clientManager.eraseClientByFD(_curEvent->ident);
    close(_curEvent->ident);
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
