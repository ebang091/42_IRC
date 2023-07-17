/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebang <ebang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 12:28:31 by ebang             #+#    #+#             */
/*   Updated: 2023/07/17 14:52:20 by ebang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ChannelManager.hpp"

void ChannelManager::joinChannel(std::string channelName, Client &client){
    //channelinfo에서 확인한 후 없으면 만듬
    if(_channelInfo.find(channelName) == _channelInfo.end()){
		_channelInfo.insert(std::make_pair(channelName, Channel(channelName, &Client)));
    }
    else{
        // 방이 +i 권한이면 못들어옴
        // 방이 +k이면 패스워드 입력해야함
        // 방에 l 옵션과 함께 limit 이 있다면, limit을 확인한다.
        
        _channelInfo[channelName]._clientList.insert(std::make_pair(client.nickname , &Client));
    }
}

ChannelManager::Channel::Channel(const std::string& channelName, Client* client)
	: _name(name)
	, _permissions(0)
	, _clientList()
    , _operator(client)
{
   time(&_creationTime);
}