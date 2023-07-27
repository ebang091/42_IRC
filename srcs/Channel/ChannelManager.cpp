/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelManager.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebang <ebang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 12:28:31 by ebang             #+#    #+#             */
/*   Updated: 2023/07/26 19:45:32 by ebang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ChannelManager.hpp"

ChannelManager::~ChannelManager(){
	std::map<std::string, Channel*>::iterator it;
    
    it = _channels.begin();
    for(; it != _channels.end(); it++){
        delete it->second;
    }
}

ChannelManager& ChannelManager::getInstance(){
	static ChannelManager instance;
	return instance;
}

void ChannelManager::insertChannel(const std::string& channelName, Client *client){
    //channelinfo에서 확인한 후 없으면 만듬
    if(_channels.find(channelName) == _channels.end()){
		  _channels.insert(std::make_pair(channelName, new Channel(channelName, client)));
    }
    else{
        _channels[channelName]->insertClient(client);
    }
}

void ChannelManager::eraseChannel(const std::string &channelName){
	std::map<std::string, Channel*>::iterator target = _channels.find(channelName);

	if (target != _channels.end())
	{
		Channel* temp = target->second;
		_channels.erase(channelName);
		delete temp;
	}
}

Channel* ChannelManager::getChannelByName(const std::string& channelName) const{
	try
	{
		Channel* tmp = _channels.at(channelName);
		return tmp;
	}
	catch(const std::exception& e)
	{
		return NULL;
	}
}

// void ChannelManager::sendClientAllChannels(const std::string& targetName){
// 	std::set<int> isSent;

// 	for (std::map<std::string, Channel*>::iterator iter = _channels.begin(); iter != _channels.end(); ++iter)
// 	{
// 		if (!iter->second->getClientByNick(targetName))
// 			continue;
		
// 		//iter->second->sendToClients(isSent);
// 	}
// }

void ChannelManager::eraseClientAllChannels(const std::string& targetName){
	std::set<int> isSent;
	std::vector<Channel*> eraseList;

	for (std::map<std::string, Channel*>::iterator iter = _channels.begin(); iter != _channels.end(); ++iter)
	{
		iter->second->eraseInvite(targetName);
		if (!iter->second->getClientByNick(targetName))
			continue;
		
		iter->second->sendToClients(isSent);
		iter->second->eraseOperator(targetName);
		if (iter->second->eraseClient(targetName) == 0)
			eraseList.push_back(iter->second);
	}

	for (size_t i = 0; i < eraseList.size(); ++i)
	{
		_channels.erase(eraseList[i]->getName());
		delete eraseList[i];
	}
}

void ChannelManager::changeNickNameAllChannels(const std::string& originname, Client *clientNewNick){
	std::set<int> isSent;
	
	for (std::map<std::string, Channel*>::iterator iter = _channels.begin(); iter != _channels.end(); ++iter)
	{
		if (!iter->second->getClientByNick(originname))
			continue;
		iter->second->eraseClient(originname);
		if(iter->second->getInviteByNick(originname) != NULL){
			iter->second->eraseInvite(originname);
			iter->second->insertClient(clientNewNick);
		}		
		if(iter->second->getOperatorByNick(originname) != NULL){
			iter->second->eraseOperator(originname);
			iter->second->insertOperator(clientNewNick);
		}
		iter->second->sendToClients(isSent);
		iter->second->insertClient(clientNewNick);
	}
}

#include <bitset>
void ChannelManager::printChannels(){
	std::cout << "\n** channel list\n";
	for (std::map<std::string, Channel*>::iterator iter = _channels.begin(); iter != _channels.end(); ++iter)
	{
		std::cout << "channel name [" << iter->second->getName() << "]\n";
		std::cout << "topic	        :" << iter->second->getTopic().__content << "\n";
		std::cout << "creationTime  :" << iter->second->getCreationTime() << "\n";
		std::cout << "limit	        :" << iter->second->getLimit() << "\n";
		std::cout << "permission    :" << std::bitset<8>(iter->second->getPermissions()) << "\n";
		std::cout << "password      :" << iter->second->getPassword() << "\n";

		
		iter->second->printClients();
	}
}