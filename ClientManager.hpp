/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebang <ebang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 12:48:26 by ebang             #+#    #+#             */
/*   Updated: 2023/07/17 17:39:19 by ebang            ###   ########.fr       */
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

class ClientManager{
private:
    std::map<std::string, Client*> _clientByNick;
    std::map<int, Client*> _clientByFD;

public:
    static ClientManager& getInstance();
    Client* getClientByNick(const std::string& target);
    Client* getClientByFD(int fd);
    void insertClientByNick(const std::string& target, Client* client);
    void insertClientByFD(int fd);
    void eraseClientByNick(const std::string& target);
    void eraseClientByFD(int fd);

    void deleteAndCloseAllClients();
};

#endif