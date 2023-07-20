/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebang <ebang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 12:48:26 by ebang             #+#    #+#             */
/*   Updated: 2023/07/20 14:53:55 by ebang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef CLIENT_HANDLER_HPP
#define CLIENT_HANDLER_HPP

#include <string>
#include <iostream>
#include <map>
#include "ErrorHandler.hpp"
#include "Client.hpp"
#include "Enums.hpp"

class ClientManager{
private:
    std::map<std::string, Client*> _clientByNick;
    std::map<int, Client*> _clientByFD;

public:
    ~ClientManager();

public:
    static ClientManager& getInstance();
    Client* getClientByNick(const std::string& nickName);
    Client* getClientByFD(int fd);
    void insertClientByNick(const std::string& nickName, Client* client);
    void insertClientByFD(int fd);
    void eraseClientByNick(const std::string& nickName);
    void eraseClientByFD(int fd);

public:
    void printClients();
};

#endif