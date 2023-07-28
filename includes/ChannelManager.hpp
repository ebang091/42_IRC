/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelManager.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebang <ebang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 12:48:26 by ebang             #+#    #+#             */
/*   Updated: 2023/07/28 16:16:20 by ebang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef CHANNEL_HANDLER_HPP
#define CHANNEL_HANDLER_HPP

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <ctime>
#include "ErrorHandler.hpp"
#include "Client.hpp"
#include "Channel.hpp"

// class Channel;

class ChannelManager{
private:
    std::map<std::string, Channel*> _channels;

private:
    ChannelManager(){};

public:
   ~ChannelManager();

public:
    static ChannelManager& getInstance();
    void insertChannel(const std::string& channelName, Client *client); //- > 이렇게 하면서 channel 객체 하나 만들고 기본값 +nt, list에는 빈 리스트
    Channel *getChannelByName(const std::string &channelName) const;
    void eraseChannel(const std::string &channelName);

public:
    void printChannels();
    void sendClientAllChannels(const std::string& targetName);
    void eraseClientAllChannels(const std::string& targetName);
    void changeNickNameAllChannels(const std::string& originname, Client *clientNewNick);
};

#endif