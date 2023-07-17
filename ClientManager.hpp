/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebang <ebang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 12:48:26 by ebang             #+#    #+#             */
/*   Updated: 2023/07/17 14:54:44 by ebang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef CLIENT_HANDLER_HPP
#define CLIENT_HANDLER_HPP

#include <string>
#include <iostream>
#include <map>
#include "ErrorHandler.hpp"

class ClientManager{
public:
    struct Client{
    public:
        //hostname
        //username
        //realname
        
        //socket number

        
    public:
        Client(const std::string& name);
    };

private:
    std::map<std::string, Client> _clientInfo;

};

#endif