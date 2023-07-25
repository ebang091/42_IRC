#pragma once
#ifndef ENUMS_HPP
#define ENUMS_HPP

#define EVENT_BUFFER_SIZE 50
#define READ_BUFFER_SIZE 1030
#define READ_MAX 1024

#define PORT_MIN 1023
#define PORT_MAX 49152

#define LISTEN_QUEUE_SIZE 15

#define CHANNEL_PREFIX '#'

#define GET_PERMISSION_N(permissions) permissions & PERMISSION::N
#define SWITCH_PERMISSION_N(permissions) permissions ^ PERMISSION::N
#define GET_PERMISSION_T(permissions) permissions & PERMISSION::T
#define SWITCH_PERMISSION_T(permissions) permissions ^ PERMISSION::T
#define	GET_PERMISSION_I(permissions) permissions & PERMISSION::I
#define SWITCH_PERMISSION_I(permissions) permissions ^ PERMISSION::I
#define GET_PERMISSION_K(permissions) permissions & PERMISSION::K
#define SWITCH_PERMISSION_K(permissions) permissions ^ PERMISSION::K
#define GET_PERMISSION_O(permissions) permissions & PERMISSION::O
#define SWITCH_PERMISSION_O(permissions) permissions ^ PERMISSION::O
#define GET_PERMISSION_L(permissions) permissions & PERMISSION::L
#define SWITCH_PERMISSION_L(permissions) permissions ^ PERMISSION::L
#define VERIFY_SUCCESS(status) (status == NUMERIC::SUCCESS) || (status == NUMERIC::WELCOME) || (status == NUMERIC::YOURHOST) || (status == NUMERIC::CREATED) || (status == NUMERIC::MYINFO) || (status == NUMERIC::ISUPPORT)

namespace PERMISSION
{
	enum CODE
	{
		N = 1,	// 0000 0001
        I = 2,	// 0000 0010
        T = 4,	// 0000 0100
        K = 8,	// 0000 1000
        O = 16,	// 0001 0000
        L = 32	// 0010 0000
	};	
};

namespace STATE{
	enum CODE {
		PLUS,
		MINUS,
		NONE,
		SIZE
	};
};

namespace INPUT{
	enum CODE {
		N,
		T,
		I,
		K,
		O,
		L,
		PLUS,
		MINUS,
		SIZE,
		NONE
	};
};

namespace CONFIG{
	enum CODE	{
		USERLIMIT = 50,
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

namespace CMD{
	enum CODE	{
		NONE = -1,
		CAP,
		QUIT,
		NICK,
		JOIN,
		KICK,
		INVITE,
		TOPIC,
		MODE,
		PART,
		PRIVMSG,
		USER,
		PASS,
		SIZE
	};
};

namespace NUMERIC{
	enum CODE	{
		// --- ERROR ---
		UNKNOWN_ERR = 400,
		NO_PARAM = 696,
		NEED_MORE_PARAM = 461,
		ALREADY_REGISTERED = 462,
		PASS_MISMATCH = 464,

		NO_SUCH_NICK = 401,
		INVALID_NICK = 432,
		DUP_NICK = 433,
		NOT_OPER = 482,

		NO_SUCH_CHAN = 403,
		TARGET_NOT_ON_CHAN = 441,
		NOT_ON_CHAN = 442,
		ALREADY_ON_CHAN = 443,
		BAD_CHAN_KEY = 475,
		KEY_ALREADY_SET = 467,
		FULL_CHANNEL = 471,
		INVALID_MODE = 472,
		INVITE_ONLY_CHAN = 473,
		BAD_CHAN_MASK = 476,
	
		NOTHING = -1,
		
		// --- SUCCESS ---
		SUCCESS = 0,
		WELCOME = 1,
		YOURHOST = 2,
		CREATED = 3,
		MYINFO = 4,
		ISUPPORT = 5,

		RPL_NAMREPLY = 353,
		RPL_ENDOFNAMES = 366,
		TOPIC = 332,
		TOPIC_WHOTIME = 333
	};
};



#endif