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
#include "EventHandler.hpp"

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

bool Parser::parseByDelimeter(char delimeter, std::string& parsingLine, std::queue<std::string> &buffer){
    std::stringstream ssLine(parsingLine);
    std::string word;
	bool isExist = false;

    while (std::getline(ssLine, word, delimeter))
    {
        if (word.back() == 13)
            word.pop_back();
		isExist = true;
        buffer.push(word);
    }
	return isExist;
}

void Parser::parseCommandsAndExecute(std::string command){
    //파싱 방식: 스페이스 단위로 parameter list를 만들어 반환받는다. 가장 앞은 CMD, 그 뒤는 parameters
    std::queue<std::string> commandsQ;
    std::string cmd;
    std::string word;
    std::vector<std::string> parameters;


    std::cout << "input command in parseCommandsAndExecute() : " << command << "\n";
    CommandHandler& commandHandler = CommandHandler::getInstance();
	Client* requestClient = EventHandler::getInstance().getRequestClient();

    //crlf 를  단위로 parse ByDeli -> CRLF  -> 
    if (!parseByDelimeter('\n', command, commandsQ))
		return requestClient->addBuffer(command);

	while(!commandsQ.empty()){
        std::stringstream ss(commandsQ.front());
        ss >> cmd;

        parameters.clear();
        while(ss >> word)
            parameters.push_back(word);
        CMD::CODE cmdCode = commandHandler.identifyCommand(cmd);//NONE 이면 무시? 에러?
        commandHandler.executeCommand(cmdCode, parameters); //실행 및 출력
        commandsQ.pop();
    }

	requestClient->clearBuffer();
}

