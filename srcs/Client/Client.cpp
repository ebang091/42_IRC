#include "Client.hpp"

Client::Client(int socketNumber)
	: _nickname("default")
    , _username("")
    , _realname("")
    , _host("")
    , _recvBuffer("")
    , _sendQue()
    , _auth(0)
    , _socketNumber(socketNumber)
{
}

Client::Client(const std::string& nickName, const std::string& userName, const std::string& host)
	: _nickname(nickName)
    , _username(userName)
    , _realname("")
    , _host(host)
    , _recvBuffer()
    , _sendQue()
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

void Client::addRecvBuffer(const std::string& str){
    this->_recvBuffer += str;
}

void Client::clearRecvBuffer(){
    this->_recvBuffer.clear();
}

const std::string& Client::getRecvBuffer(){
    return this->_recvBuffer;
}

std::queue<std::string>& Client::getSendQue(){
	return this->_sendQue;
}

bool Client::checkHost(int clientSocket, const std::string& newHost){
    struct sockaddr_in clnt_addr;
    socklen_t size = sizeof(clnt_addr);

    getsockname(clientSocket, &(struct sockaddr& )clnt_addr, &size);
    std::string host = inet_ntoa(clnt_addr.sin_addr);
    
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
    this->_auth = s;
}