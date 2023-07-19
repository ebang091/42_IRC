/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebang <ebang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 12:28:31 by ebang             #+#    #+#             */
/*   Updated: 2023/07/14 13:12:23 by ebang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClientManager.hpp"

ClientManager::~ClientManager(){
	std::map<int, Client*>::iterator it;
    
    it = _clientByFD.begin();
    for(; it != _clientByFD.end(); it++){
        delete it->second;
        close(it->first);
    }
}

ClientManager& ClientManager::getInstance(){
	static ClientManager instance;
	return instance;
}

Client* ClientManager::getClientByNick(const std::string& nickName){
	try
	{
		Client* tmp = _clientByNick.at(nickName);		
		return tmp;
	}
	catch(const std::exception& e)
	{
		return NULL;
	}
}

Client* ClientManager::getClientByFD(int fd){
	try
	{
		Client* tmp = _clientByFD.at(fd);		
		return tmp;
	}
	catch(const std::exception& e)
	{
		return NULL;
	}
}

void ClientManager::insertClientByNick(const std::string& nickName, Client* client){
	_clientByNick.insert(std::make_pair(nickName, client));
}

void ClientManager::insertClientByFD(int fd){
	_clientByFD.insert(std::make_pair(fd, new Client(fd)));
}

void ClientManager::eraseClientByNick(const std::string& nickName){
	std::map<std::string, Client*>::iterator target = _clientByNick.find(nickName);

	if (target != _clientByNick.end())
	{
		delete target->second;
		_clientByNick.erase(nickName);
	}
}

void ClientManager::eraseClientByFD(int fd){
	std::map<int, Client*>::iterator target = _clientByFD.find(fd);

	if (target != _clientByFD.end())
	{
		delete target->second;
		_clientByFD.erase(fd);
	}
}