#include "Client.hpp"

Client::Client(int socketNumber)
	: _socketNumber(socketNumber),
    _nickname("default")

{
}

const std::string& Client::getNickName() const{
    return this->_nickname;
}

const std::string& Client::getUserName() const{
    return this->_username;
}

const std::string& Client::getRealName() const{
    return this->_realname;
}

const std::string& Client::getIpAddress() const{
    return this->_ipAddress;
}

int Client::getSocketNumber() const{
    return this->_socketNumber;
}

void Client::setNickName(const std::string& newNickName){
    //호출:  처음 접속 시에 map <fd, Client> 저장용
    //      변경 시에 	getByFd  해서 map<nickname, Client> 
    //  묶이는 정보 : socket number

   this->_nickname = newNickName;
}

void Client::setUserName(const std::string& newUserName){ 
   this->_username = newUserName;
}

void Client::setRealName(const std::string& newRealName){ 
   this->_realname = newRealName;
}

void Client::setIpAddress(int clientSocket){
    struct sockaddr_in clnt_addr;
    socklen_t size = sizeof(clnt_addr);

    getsockname(clientSocket, &(struct sockaddr& )clnt_addr, &size);
    char* result = inet_ntoa(clnt_addr.sin_addr);
    _ipAddress = result;
    free(result);
}