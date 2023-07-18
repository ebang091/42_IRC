#include "Client.hpp"

Client::Client(int socketNumber)
	: _socketNumber(socketNumber)
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

int Client::getSocketNumber() const{
    return this->_socketNumber;
}

void Client::SetNickName(const std::string& newNickName){
    //호출:  처음 접속 시에 map <fd, Client> 저장용
    //      변경 시에 	getByFd  해서 map<nickname, Client> 
    //  묶이는 정보 : socket number

   this->_nickname = newNickName;
}

void Client::SetUserName(const std::string& newUserName){ 
   this->_username = newUserName;
}

void Client::SetRealName(const std::string& newRealName){ 
   this->_realname = newRealName;
}