/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelManager.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebang <ebang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 12:48:26 by ebang             #+#    #+#             */
/*   Updated: 2023/07/18 18:00:27 by ebang            ###   ########.fr       */
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
#include "Client.hpp"
#include "Channel.hpp"

#define GET_PERMISSION_N(permissions) permissions & Permission::N
#define SET_PERMISSION_N(permissions) permissions |= Permission::N
#define GET_PERMISSION_T(permissions) permissions & Permission::T
#define SET_PERMISSION_T(permissions) permissions |= Permission::T
#define GET_PERMISSION_I(permissions) permissions & Permission::I
#define SET_PERMISSION_I(permissions) permissions |= Permission::I
#define GET_PERMISSION_K(permissions) permissions & Permission::K
#define SET_PERMISSION_K(permissions) permissions |= Permission::K
#define GET_PERMISSION_O(permissions) permissions & Permission::O
#define SET_PERMISSION_O(permissions) permissions |= Permission::O
#define GET_PERMISSION_L(permissions) permissions & Permission::L
#define SET_PERMISSION_L(permissions) permissions |= Permission::L

namespace Permission
{
	enum Permission
	{
		N = 1,
        I = 2,
        T = 4,
        K = 8,
        O = 16,
        L = 32
	};	
};

class ChannelManager{
private:
    std::map<std::string, Channel> _channelInfo;//Channel 포인터로 바꿀까요?

      /*
       client 리스트 출력하기 (방장)

       //topic 출력하기

       //topic 바꾸기. (t 옵션 확인)
       그 채널안에 있는 유저야 함
       채널에 t 옵션이 있으면 operator 여야 함.

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
        static ChannelManager& getInstance();
        void insertChannel(const std::string& channelName, Client *client); //- > 이렇게 하면서 channel 객체 하나 만들고 기본값 +nt, list에는 빈 리스트
        Channel *getChannelByName(const std::string &channelName);
        void eraseChannel(const std::string &channelName);
};

#endif