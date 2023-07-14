#include "SocketHandler.hpp"

SocketHandler& SocketHandler::getInstance(){
	static SocketHandler instance;
	return instance;
}

int SocketHandler::getServerSocket(){
	return _serverSocket;
}

void SocketHandler::makeAndListenSocket(int portNumber){
    struct sockaddr_in server_addr;
    
    if ((_serverSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
        throw ErrorHandler::SocketException();
           
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(portNumber);

    if (bind(_serverSocket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
        throw ErrorHandler::BindException();

    if (listen(_serverSocket, LISTEN_QUEUE_SIZE) == -1)
        throw ErrorHandler::ListenException();

	int original = fcntl(_serverSocket, F_GETFL, 0);
    if (original == -1 || fcntl(_serverSocket, F_SETFL, original | O_NONBLOCK) == -1)
		throw ErrorHandler::FcntlException();

    return ;    
}
