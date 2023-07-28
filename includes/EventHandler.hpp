/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventHandler.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebang <ebang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 13:18:51 by ebang             #+#    #+#             */
/*   Updated: 2023/07/28 16:17:32 by ebang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#pragma once
#ifndef EVENT_HANDLER_HPP
#define EVENT_HANDLER_HPP

#include <sys/types.h>
#include <sys/event.h>
#include <string>
#include <vector>
#include <map>
#include "ErrorHandler.hpp"
#include "SocketHandler.hpp"
#include "Parser.hpp"
#include "ClientManager.hpp"
#include "Enums.hpp"

class EventHandler{
private:
    Client *_requestClient;
    Channel *_requestChannel;
    std::vector<struct kevent> _changeList;
    struct kevent _event_list[EVENT_BUFFER_SIZE];
    struct kevent* _curEvent;
    int _numberOfNewEvents;
    int _kq;
    int _serverSocket;

private:
    EventHandler();

private:
    void init();
    void disconnectCurClient(struct kevent* curEvent);
	void changeEvents(std::vector<struct kevent>& changeList, uintptr_t ident, int16_t filter,\
                    uint16_t flags, uint32_t fflags, intptr_t data, void *udata);

public:
	static EventHandler& getInstance();
    void listenToClients();
    Client *getRequestClient() const;
    Channel *getRequestChannel()const;
    void setRequestChannel(Channel* channel);
};


#endif