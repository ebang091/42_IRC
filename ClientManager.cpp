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

ClientManager& ClientManager::getInstance(){
	static ClientManager instance;
	return instance;
}

Client* ClientManager::getClientByNick(const std::string& target){
	// exception 발생
	return _clientByNick.at(target);
}

Client* ClientManager::getClientByFD(int fd){
	// exception 발생
	return _clientByFD.at(fd);
}

void ClientManager::insertClientByNick(const std::string& target, Client* client){
	_clientByNick.insert(std::make_pair(target, client));
}

void ClientManager::insertClientByFD(int fd){
	_clientByFD.insert(std::make_pair(fd, new Client(fd)));
}

void ClientManager::eraseClientByNick(const std::string& target){
	_clientByNick.erase(target);
}

void ClientManager::eraseClientByFD(int fd){
	_clientByFD.erase(fd);
}

void ClientManager::deleteAndCloseAllClients(){
    std::map<int, Client*>::iterator it;
    
    it = this->_clientByFD.begin();
    for(; it != this->_clientByFD.end(); it++){
        delete it->second;
        close(it->first);
    }
}