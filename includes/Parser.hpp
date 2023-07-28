/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebang <ebang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 12:48:26 by ebang             #+#    #+#             */
/*   Updated: 2023/07/14 13:12:29 by ebang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <cstring>
#include <iostream>
#include <sstream>
#include <vector>
#include "ErrorHandler.hpp"
#include "CommandHandler.hpp"
#include "EventHandler.hpp"

class Parser{
private:
	Parser(){};

public:
	static Parser& getInstance();
	bool getCmdQ(std::string& parsingLine, std::queue<std::string> &buffer);
	int parsePortNumber(std::string portnumber);
	void parseCommandsAndExecute(std::string command);
	void parseByDelimeter(char delimeter, std::string& parsingLine, std::queue<std::string> &buffer);
};

#endif