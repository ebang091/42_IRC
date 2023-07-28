#pragma once
#ifndef BOT_HPP
#define BOT_HPP

#include <sstream>
#include <cstring>
#include <set>
#include "Client.hpp"
#include "Enums.hpp"

class CommandHandler;
class Channel;
class MessageHandler;
class ChannelManager;

class Bot
{
private:
    std::string _name;
	std::string _description;
    Client *_requestClient;
    Channel *_requestChannel;
    ChannelManager *_channelManager;
	MessageHandler *_messageHandler;

private:
	Bot();

private:
	BOT::CODE parseMessage(std::vector<std::string>& parameters);
	const std::string& getNotice() const;
	std::string getSendingMessage(BOT::CODE code);

public:
	static Bot& getInstance();
	const std::string& getName() const;
	void sendMessage(std::vector<std::string>& parameters, Client* requestClient);
    void sendWelcomeMessage(std::string& channelName);
};

#endif

/*
    IRC Bot ?
     : IRC 네트워크에  "클라이언트로 접속"  하는 (독립된 스크립트 또는 프로그램)으로, 
     사용자들 입장에서는  "또 다른 사용자"  로 표시된다. 
     IRC 봇은 일반적인 사용자와는 달리  "사용자의 입력을 받아 자동화된 기능을 수행"  한다.
     : IRC 기반 채팅방 또는 채널 내에서 자동화 된 작업을 수행하는 응용 프로그램 유형입니다. 
     사용자 대신 여러 채널 특정 작업을 수행 할 수 있으며 일반적으로 채널 운영자로 구성됩니다.
     : 대화방 / 채널에서 항상 연결되어 있거나 사용 가능한 사용자로 표시되는 채널의 활동을 유지하는 데 사용됩니다.
    // -- PRIVMSG --
    // set notice
    // get notice
    // set welcome
    // get welcome
    // join Success -> getWelcome(), getNotice()
    요청 : 유저 -> 서버
    데이터 보내기 : 서버 -> 유저
         서버
    유저 -    - 봇
    0. 서버가 시작 시 봇을 클라이언트 리스트에 삽입
    1. 유저가 봇에게 귓속말을 보냄
    2. 유저의 메시지를 분석하여 targetName이 봇이라면 봇 실행
    3. 봇 클래스에서 전달받은 문자열을 파싱하여 채널에서 정보를 가져오고 만들어진 문자열을 유저에게 send()
    * 꼭 채널에 없어도 귓속말을 보낼 수 있으므로, 항상 메시지에 채널도 같이 넣어줘야 할 듯
*/