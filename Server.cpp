/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebang <ebang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 12:28:31 by ebang             #+#    #+#             */
/*   Updated: 2023/07/14 15:01:39 by ebang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <string>

Server::Server(){
    
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
        _portNumber = parser.parsePortNumber(portnumber);
        //socket(), bind(), listen()
        socketHandler.makeAndListenSocket(_portNumber);
        eventHandler.listenToClients();
		std::cout << socketHandler.getServerSocket() << "\n";
        
        //이벤트 관리하면서 kevent 발생 시 client 들음. (발생 가능 오류 : bind, listen)
        //(서버 소켓일 때는 연결, fcntl, accept) (발생 가능 오류 : accpet)
        //(클라이언트 소켓일 때는) 파싱 -> 실행 (발생 가능 오류 : 파싱,  없는 클라이언트 소켓인데 들어옴)
        
    }
    catch (const std::exception& e)
    {
        //오류 잡음
		std::cout << e.what() << std::endl;
    }
	// close
}