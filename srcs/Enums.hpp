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
		CHANLIMIT = 20,
		CHANNELLEN = 64,
		KEYLEN = 32,
		KICKMSGLEN = 255,
		TOPICLEN = 307,
		USERLEN = 10,
		HOSTLEN = 64,
		NAMELEN = 128,
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

namespace NUMERIC
{
	enum CODE
	{
		// --- ERROR ---
		NO_PARAM = 696,

		NO_SUCH_NICK = 401,
		INVALID_NICK = 432,
		DUP_NICK = 433,
		NOT_OPER = 482,

		NO_SUCH_CHAN = 403,
		ALREADY_ON_CHAN = 443,
		INCORRECT_KEY = 475,
		KEY_ALREADY_SET = 467,
		INVALID_MODE = 472,

		// --- SUCCESS ---
		SUCCESS = 0,
		NOTHING = -1,
		WELCOME = 1,
		YOURHOST = 2,
		CREATED = 3,
		MYINFO = 4,
		ISUPPORT = 5
	};
};

#endif