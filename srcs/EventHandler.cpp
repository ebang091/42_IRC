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

void EventHandler::listenToClients(){
    int serverSocket = SocketHandler::getInstance().getServerSocket();
    Parser &parser = Parser::getInstance();
    ClientManager &clientManager = ClientManager::getInstance();
    struct kevent event_list[EVENT_BUFFER_SIZE]; // kevent array for eventlist
    int kq;
    int numberOfNewEvents;
    struct kevent* curEvent;
    
    if ((kq = kqueue()) == -1)
        throw ErrorHandler::KqueueException();

    changeEvents(_changeList, serverSocket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);

    while (1)
    {
        numberOfNewEvents = kevent(kq, &_changeList[0], _changeList.size(), event_list, EVENT_BUFFER_SIZE, NULL);
        if (numberOfNewEvents == -1)
            throw ErrorHandler::KeventException();

        _changeList.clear(); // clear changeList for new changes

        for (int i = 0; i < numberOfNewEvents; ++i){
            curEvent = &event_list[i];

             /* check error event return */
            if (curEvent->flags & EV_ERROR)
            {
                if (curEvent->ident == serverSocket){
					throw ErrorHandler::KeventException();
                }
                else
                {
                    //어쩌면 quit 일 때도 이럴 수도..?
                    close(curEvent->ident);
                    clientManager.eraseClientByFD(curEvent->ident);
                }
            }
			else if (curEvent->filter == EVFILT_READ)
			{
				if (curEvent->ident == serverSocket)
                {
                    /* accept new client */
                    int clientSocket;
                    
                    if ((clientSocket = accept(serverSocket, NULL, NULL)) == -1)
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
					int n = recv(curEvent->ident, buf, READ_BUFFER_SIZE, MSG_DONTWAIT);
                    buf[n] = '\0';
                    this->_requestClient = clientManager.getClientByFD(curEvent->ident); 

					if (n == -1){
						close(curEvent->ident);
						clientManager.eraseClientByFD(curEvent->ident);
                	}
                	else if (n > READ_MAX){
                	    //안된다고 출력하고 아무 반응도 안함.                    	
                	}
                	else{
                        std::string str = "okay";
                        
                	    parser.parseCommandsAndExecute(buf);//파싱하고 실행
                        test();
                        //명령을 실행
                        
                        
						send(curEvent->ident, str.c_str(), str.length(), MSG_DONTWAIT);
                        //std::cout << "write to client : " << buf << "\n";
                	}
                }
			}
        }

    }
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
