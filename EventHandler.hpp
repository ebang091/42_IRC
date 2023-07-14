/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventHandler.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebang <ebang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 13:18:51 by ebang             #+#    #+#             */
/*   Updated: 2023/07/14 14:41:41 by ebang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#pragma once
#ifndef EVENT_HANDLER_HPP
#define EVENT_HANDLER_HPP
#define EVENT_BUFFER_SIZE 50
#define READ_BUFFER_SIZE 1030
#define READ_MAX 1024

#include <sys/types.h>
#include <sys/event.h>
#include <string>
#include <vector>
#include <map>
#include "ErrorHandler.hpp"
#include "SocketHandler.hpp"
#include "Parser.hpp"
#include <iostream>

class EventHandler{
private:
    EventHandler();
    std::vector<struct kevent> _changeList;
	std::map<int, bool> _clients;
    
private:
	void changeEvents(std::vector<struct kevent>& changeList, uintptr_t ident, int16_t filter,\
                    uint16_t flags, uint32_t fflags, intptr_t data, void *udata);
	void closeAllClients();

public:
	static EventHandler& getInstance();	// singleton    
    //kqueue를 만들고 kevent로 듣기. 그 안에서 클라이언트 통신. 파싱. 실행.
    void listenToClients();
};


#endif