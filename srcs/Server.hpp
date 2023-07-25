/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eunjungbang <eunjungbang@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 12:28:35 by ebang             #+#    #+#             */
/*   Updated: 2023/07/25 23:53:17 by eunjungbang      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include "SocketHandler.hpp"
#include "EventHandler.hpp"
#include "Parser.hpp"

class Server{
private:
    //TODO
    //소켓 관리
    //이벤트 관리
    //클라이언트 관리 - 1. 파싱 2. 실행

    //생성시각
    /*
    AWAYLEN=200 CASEMAPPING=rfc1459 CHANLIMIT=#:20 CHANMODES=b,k,l,imnpst CHANNELLEN=64 CHANTYPES=# ELIST=CMNTU HOSTLEN=64 KEYLEN=32 KICKLEN=255 LINELEN=512 MAXLIST=b:100 :are supported by this server
    MAXTARGETS=20 MODES=20 NAMELEN=128 NETWORK=Localnet NICKLEN=30 PREFIX=(ov)@+ SAFELIST STATUSMSG=@+ TOPICLEN=307 USERLEN=10 USERMODES=,,s,iow WHOX :are supported by this server
    */
    time_t _creationTime;
    int _portNumber;
	std::string _passWord;
    Server();

public:
	static Server& getInstance();	// singleton
	int getPortNumber();
	const std::string& getPassword();
    void run(std::string portnumber, std::string password);
};


#endif