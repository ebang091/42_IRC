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

bool Parser::getCmdQ(std::string& parsingLine, std::queue<std::string> &buffer){
    Client* requestClient = EventHandler::getInstance().getRequestClient();
    std::string word;

    if (parsingLine.size() < 2 || (parsingLine[parsingLine.size() - 2] != 13 && parsingLine[parsingLine.size() - 1] != 10))
        return false;

    parsingLine = requestClient->getRecvBuffer() + parsingLine;
    std::stringstream ssLine(parsingLine);

    while (std::getline(ssLine, word, '\n'))
    {
        if (word.back() == 13)
            word.pop_back();
        buffer.push(word);
    }
	return true;
}

void Parser::parseByDelimeter(char delimeter, std::string& parsingLine, std::queue<std::string> &buffer){
    std::string word;
    std::stringstream ssLine(parsingLine);

    while (std::getline(ssLine, word, delimeter))
        buffer.push(word);
}

void Parser::parseCommandsAndExecute(std::string command){
    std::queue<std::string> commandsQ;
    std::string cmd;
    std::string word;
    std::vector<std::string> parameters;

    CommandHandler& commandHandler = CommandHandler::getInstance();
	Client* requestClient = EventHandler::getInstance().getRequestClient();

    if (!getCmdQ(command, commandsQ))
		return requestClient->addRecvBuffer(command);

	while(!commandsQ.empty()){
        std::stringstream ss(commandsQ.front());
        ss >> cmd;

        parameters.clear();
        while(ss >> word)
            parameters.push_back(word);
        CMD::CODE cmdCode = commandHandler.identifyCommand(cmd);
        commandHandler.executeCommand(cmdCode, parameters);
        commandsQ.pop();
    }

	requestClient->clearRecvBuffer();
}

