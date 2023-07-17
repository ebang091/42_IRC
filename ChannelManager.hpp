/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebang <ebang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 12:48:26 by ebang             #+#    #+#             */
/*   Updated: 2023/07/17 14:52:20 by ebang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef CHANNEL_HANDLER_HPP
#define CHANNEL_HANDLER_HPP

#include <string>
#include <iostream>
#include <map>
#include <ctime>
#include "ErrorHandler.hpp"
#include "ClientHandler.hpp"

namespace Permission
{
	enum Permission
	{
		N, I, T, K, O, L
	};	
};

class ChannelManager{
private:
    struct Channel{
	public:
		std::string _name;
    	short _permissions;
		std::map<std::string, Client*> _clientList;
        time_t _creationTime;
        Client *_operator; //없으면 NULL

	public:
		Channel(const std::string& channelame, Client* client);
    };

    std::map<std::string, Channel> _channelInfo;

      /*
       client 리스트 출력하기 (방장)
       //새 유저가 join 시
            //새로운 유저가 들어오면 client 리스트들에게 이 유저를 출력한다.
            //새로운 유저에게 이 리스트를 출력한다. 
        
            
        client 내보내기
            //이미 있는 유저면 내보내기 가능
            
        client 초대하기
            //server에 있는 유저면 초대 가능
            //없으면 401 NO SUCH NICK

        설정 변경 MODE
            초대만 가능한 방 설정 (i)
                //그냥 들어오려고 하면 473 one #newRoom :Cannot join channel (invite only)
            topic 설정, 잠그기 (t)
                //
            채널 패스워드 설정하기, 없애기 (k)
            채널 op 권한 주기, 뺏기
     */

    public:
        void joinChannel(); //- > 이렇게 하면서 channel 객체 하나 만들고 기본값 +nt, list에는 빈 리스트 
};

#endif