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

int Parser::parsePortNumber(std::string portnumber){
    int value = 0;
    bool isValid = true;
	int i = 0;
    char portNumber[1024];

    
    portnumber.copy(portNumber, portnumber.size(), 0);
    portNumber[portnumber.size()] = '\0';
    while (portNumber[i] != '\0'){
        if(std::isdigit(portNumber[i])){
            value = value * 10 + portNumber[i] - '0';
        }
        else{
            isValid = false;
            break;
        }
		++i;
    }
    if(isValid == false || value < PORT_MIN || value > PORT_MAX)
        throw ErrorHandler::PortNmberException();
    
    return value;
}


void Parser::parseCommands(char *command){
    //파싱 방식: 스페이스 단위로 parameter list를 만들어 반환받는다. 가장 앞은 CMD, 그 뒤는 parameters
    std::string cmd;
    std::string word;
    std::vector<std::string> parameters;
    std::stringstream ss(command);

    CommandHandler& commandHandler = CommandHandler::getInstance();

    ss >> cmd;  
    while(ss >> word){
        parameters.push_back(cmd);
    }
    CMD::CODE cmdCode = commandHandler.identifyCommand(cmd);//NONE 이면 무시? 에러?
    commandHandler.executeCommand(cmdCode, parameters); //실행 및 출력
    
    
    

   
   //첫 접속

   //CAP LS -> 갖고 있는 MAX_LEN 들, 가용 용량 send
   //NICK nickname 



  

}

