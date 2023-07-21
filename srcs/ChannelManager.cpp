/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelManager.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebang <ebang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 12:28:31 by ebang             #+#    #+#             */
/*   Updated: 2023/07/21 17:18:57 by ebang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ChannelManager.hpp"

ChannelManager::~ChannelManager()
{
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
        // 방이 +i 권한이면 못들어옴
        // 방이 +k이면 패스워드 입력해야함
        // 방에 l 옵션과 함께 limit 이 있다면, limit을 확인한다.
        _channels[channelName]->insertClient(client);
    }
    int per;
    GET_PERMISSION_I(per);
}

void ChannelManager::eraseChannel(const std::string &channelName)
{
	std::map<std::string, Channel*>::iterator target = _channels.find(channelName);

	if (target != _channels.end())
	{
		Channel* temp = target->second;
		_channels.erase(channelName);
		delete temp;
	}
}

Channel* ChannelManager::getChannelByName(const std::string& channelName) const
{
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

void ChannelManager::eraseClientAllChannels(const std::string& targetName)
{
	std::set<int> isSent;
	std::vector<Channel*> eraseList;

	for (std::map<std::string, Channel*>::iterator iter = _channels.begin(); iter != _channels.end(); ++iter)
	{
		iter->second->eraseInvite(targetName);
		if (!iter->second->getClientByNick(targetName))
			continue;
		
		//iter->second->sendToClients(isSent);
		iter->second->eraseOperator(targetName);
		if (iter->second->eraseClient(targetName) == 0)
			eraseList.push_back(iter->second);
	}

	for (int i = 0; i < eraseList.size(); ++i)
	{
		_channels.erase(eraseList[i]->getName());
		delete eraseList[i];
	}
}

#include <bitset>
void ChannelManager::printChannels()
{
	std::cout << "\n** channel list\n";
	for (std::map<std::string, Channel*>::iterator iter = _channels.begin(); iter != _channels.end(); ++iter)
	{
		std::cout << "channel name [" << iter->second->getName() << "]\n";
		std::cout << "topic	        :" << iter->second->getTopic() << "\n";
		std::cout << "creationTime  :" << iter->second->getCreationTime() << "\n";
		std::cout << "limit	        :" << iter->second->getLimit() << "\n";
		std::cout << "permission    :" << std::bitset<8>(iter->second->getPermissions()) << "\n";
		std::cout << "password      :" << iter->second->getPassword() << "\n";

		
		iter->second->printClients();
	}
}