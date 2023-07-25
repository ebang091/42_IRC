/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eunjungbang <eunjungbang@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 12:28:31 by ebang             #+#    #+#             */
/*   Updated: 2023/07/25 23:53:28 by eunjungbang      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(){
    time(&_creationTime);
}

Server& Server::getInstance(){
	static Server instance;
	return instance;
}

int Server::getPortNumber(){
	return _portNumber;
}

const std::string& Server::getPassword(){
	return _passWord;
}

void Server::run(std::string portnumber, std::string password){
    //portnumber : socket 
    Parser& parser = Parser::getInstance();
    SocketHandler& socketHandler = SocketHandler::getInstance();
    EventHandler& eventHandler = EventHandler::getInstance();
    try
    {
        //parse portnumber
        _portNumber = parser.parsePortNumber(portnumber.c_str());
        socketHandler.makeSocketAndListen(_portNumber);
        eventHandler.listenToClients();
        
        // 이벤트 관리하면서 kevent 발생 시 client 들음. (발생 가능 오류 : bind, listen)
        // (서버 소켓일 때는 연결, fcntl, accept) (발생 가능 오류 : accpet)
        // (클라이언트 소켓일 때는) 파싱 -> 실행 (발생 가능 오류 : 파싱,  없는 클라이언트 소켓인데 들어옴)
    }
    catch (const std::exception& e)
    {
        //오류 잡음
		std::cout << e.what() << std::endl;
    }
}