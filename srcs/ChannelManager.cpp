/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelManager.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebang <ebang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 12:28:31 by ebang             #+#    #+#             */
/*   Updated: 2023/07/18 17:51:03 by ebang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ChannelManager.hpp"

ChannelManager& ChannelManager::getInstance(){
	static ChannelManager instance;
	return instance;
}

void ChannelManager::insertChannel(const std::string& channelName, Client *client){
    //channelinfo에서 확인한 후 없으면 만듬
    if(_channelInfo.find(channelName) == _channelInfo.end()){
		  _channelInfo.insert(std::make_pair(channelName, Channel(channelName, client)));
    }
    else{
        // 방이 +i 권한이면 못들어옴
        // 방이 +k이면 패스워드 입력해야함
        // 방에 l 옵션과 함께 limit 이 있다면, limit을 확인한다.
        
        _channelInfo[channelName]._clientList.insert(std::make_pair(client->getNickName() , client));
    }
    int per;
    GET_PERMISSION_I(per);
}

void ChannelManager::eraseChannel(const std::string &channelName)
{
	_channelInfo.erase(channelName);
}

Channel* ChannelManager::getChannelByName(const std::string& channelName)
{
	try
	{
		Channel* tmp = &(_channelInfo.at(channelName));
		return tmp;
	}
	catch(const std::exception& e)
	{
		return NULL;
	}
}
