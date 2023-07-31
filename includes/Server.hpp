/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebang <ebang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 12:28:35 by ebang             #+#    #+#             */
/*   Updated: 2023/07/31 16:29:19 by ebang            ###   ########.fr       */
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
    time_t _creationTime;
    int _portNumber;
	std::string _passWord;

private:
    Server();

public:
	static Server& getInstance();
	int getPortNumber();
	const std::string& getPassword();
    void run(std::string portnumber, std::string password);
};

#endif