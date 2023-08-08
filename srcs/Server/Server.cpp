/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwsong <kwsong@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 12:28:31 by ebang             #+#    #+#             */
/*   Updated: 2023/08/08 16:30:56 by kwsong           ###   ########.fr       */
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
    Parser& parser = Parser::getInstance();
    SocketHandler& socketHandler = SocketHandler::getInstance();
    EventHandler& eventHandler = EventHandler::getInstance();
    _passWord = password;
        
	_portNumber = parser.parsePortNumber(portnumber.c_str());
    socketHandler.makeSocketAndListen(_portNumber);
    eventHandler.listenToClients();
}