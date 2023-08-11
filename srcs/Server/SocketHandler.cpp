#include "SocketHandler.hpp"

SocketHandler& SocketHandler::getInstance(){
	static SocketHandler instance;
	return instance;
}

int SocketHandler::getServerSocket(){
	return _serverSocket;
}

void SocketHandler::makeSocketAndListen(int portNumber){
    struct sockaddr_in server_addr;
    
	// IPv4 인터넷 프로토콜 체계, 연결지향형 소켓 타입, IPv4 연결지향형 프로토콜
    if ((_serverSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
        throw ErrorHandler::SocketException();
    
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;	// IPv4 프로토콜 주소 체계
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);	// host to network (ulong), 컴퓨터의 IP주소가 자동으로 할당
    server_addr.sin_port = htons(portNumber);	// host to network (ushort)

    socklen_t option = 1;
	// 이미 사용된 주소를 재사용 하도록 함
	if (setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) == -1)
		throw ErrorHandler::SocketException();

    if (bind(_serverSocket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
        throw ErrorHandler::BindException();

    if (fcntl(_serverSocket, F_SETFL, O_NONBLOCK) == -1)
		throw ErrorHandler::FcntlException();

    if (listen(_serverSocket, LISTEN_QUEUE_SIZE) == -1)
        throw ErrorHandler::ListenException();

    return ;    
}
