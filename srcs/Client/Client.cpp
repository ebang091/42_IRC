#include "Client.hpp"

Client::Client(int socketNumber)
	: _nickname("default")
    , _username("")
    , _realname("")
    , _host("")
    , _recvBuffer("")
    , _auth(0)
    , _socketNumber(socketNumber)
{
}

Client::Client(const std::string& nickName, const std::string& userName, const std::string& host)
	: _nickname(nickName)
    , _username(userName)
    , _realname("")
    , _host(host)
    , _recvBuffer("")
    , _auth(0)
    , _socketNumber(1)
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

const std::string& Client::getHost() const{
    return this->_host;
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

void Client::setHost(const std::string& newHost){
    this->_host = newHost;
}

void Client::addBuffer(const std::string& str){
    this->_recvBuffer += str;
}

void Client::clearBuffer(){
    this->_recvBuffer.clear();
}

const std::string& Client::getBuffer(){
    return this->_recvBuffer;
}

#include <iostream>
bool Client::checkHost(int clientSocket, const std::string& newHost){
    struct sockaddr_in clnt_addr;
    socklen_t size = sizeof(clnt_addr);

    getsockname(clientSocket, &(struct sockaddr& )clnt_addr, &size);
    std::string host = inet_ntoa(clnt_addr.sin_addr);
    
    std::cout << "check host : " << host << ", " << newHost << "\n";
    if(host != newHost)
        return false;
    return true;
}

bool Client::authNoSent() const{
    if((GET_PASS_AUTH(this->_auth)) && (GET_USER_AUTH(this->_auth)) && (GET_NICK_AUTH(this->_auth)) && !(GET_SENT_AUTH(this->_auth)))
        return true;
    return false;
}

bool Client::isAuthenticated() const{
    if(GET_SENT_AUTH(this->_auth))
        return true;
    return false;
}

char Client::getAuth() const{
    return this->_auth;
}

void Client::setAuth(const char s){
    std::cout << "set value in SetAuth()" << s <<"\n";
    std::cout << "auth value in client: " << _auth <<"\n";
    this->_auth = s;
}