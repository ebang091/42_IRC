/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebang <ebang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 12:28:31 by ebang             #+#    #+#             */
/*   Updated: 2023/07/14 13:12:23 by ebang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

Parser& Parser::getInstance(){
	static Parser instance;
	return instance;
}

int Parser::parsePortNumber(std::string portNumber){
    int value = 0;
    bool isValid = true;

    for(size_t i = 0; i < portNumber.size(); i++){
        if(std::isdigit(portNumber[i])){
            value = value * 10 + portNumber[i] - '0';
        }
        else{
            isValid = false;
            break;
        }
    }
    if(isValid == false || value < PORT_MIN || value > PORT_MAX)
        throw ErrorHandler::PortNmberException();
    
    //포트넘버 객체에 반환..(int형 반환값)
    return value;
}

void Parser::parseCommands(char *command){
    //파싱 방식: 스페이스 단위로 parameter list를 만들어 반환받는다. 가장 앞은 CMD, 그 뒤는 parameters
    //


   
   //첫 접속

   //CAP LS -> 갖고 있는 MAX_LEN 들, 가용 용량 send
   //NICK nickname 

   /*
   파싱 방식: 스페이스 단위로 parameter list를 만들어 반환받는다.

   -> parameter의 개수가 0개이면 (ERR_NONICKNAMEGIVEN)
   -> 1개보다 크면 스페이스바가 있다는 뜻이므로 invalid

   -> 서버에서 이미 쓰이는 닉네임인지 (ERR_NICKNAMEINUSE)
   -> invalid 닉네임인지(ERR_ERRONEUSNICKNAME)
   -> valid check : 1개이상의 alphanumeric character, {,}, \, |
                    - 앞에 #, #&, &# 불가
                    - 앞에 : 불가
                    - 중간에 스페이스 불가.
    -> 갖고 있는 NICKLEN 이하의 개수인지
   */


    //


    //USER 
    /*
        USER root root 127.0.0.1 :root

        USER <username> 0 * <realname>
        realname은 
    */

   //JOIN
   /*
    요청한 클라이언트 socket number에 대해서 join 시킴. (+i)
    ChannelManager::joinChannel();
   */

  

}

