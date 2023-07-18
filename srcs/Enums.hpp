#pragma once
#ifndef ENUMS_HPP
#define ENUMS_HPP

#define EVENT_BUFFER_SIZE 50
#define READ_BUFFER_SIZE 1030
#define READ_MAX 1024

#define PORT_MIN 1023
#define PORT_MAX 49152

#define LISTEN_QUEUE_SIZE 15

namespace CONFIG
{
	enum CODE
	{
		NICKLEN = 30
	};
};

namespace CMD
{
	enum CODE
	{
		NONE = -1,
		CAP,
		QUIT,
		NICK,
		JOIN,
		KICK,
		INVITE,
		TOPIC,
		MODE,
		SIZE
	};
};

namespace ERROR
{
	enum CODE
	{
		SUCCESS = 0,
		NOTHING = 1,
		NO_PARAM = 696,

		INVALID_NICK = 432,
		DUP_NICK = 433,


	};
};

#endif