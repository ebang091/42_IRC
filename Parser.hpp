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
#define PORT_MIN 1023
#define PORT_MAX 49152

#include <string>
#include <cstring>
#include <iostream>
#include "ErrorHandler.hpp"

class Parser{
private:
    
public:
	static Parser& getInstance();
	int parsePortNumber(std::string portNumber);
	void parseCommands(char *command);
};


#endif