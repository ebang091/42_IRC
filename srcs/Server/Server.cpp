/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebang <ebang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 12:28:31 by ebang             #+#    #+#             */
/*   Updated: 2023/07/31 19:37:56 by ebang            ###   ########.fr       */
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
    try
    {
        _portNumber = parser.parsePortNumber(portnumber.c_str());
        socketHandler.makeSocketAndListen(_portNumber);
        eventHandler.listenToClients();
        
    }
    catch (const std::exception& e)
    {
        std::cout << "here1";
		std::cout << e.what() << std::endl;
    }
}