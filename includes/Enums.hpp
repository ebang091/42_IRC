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
#define DESCRIPT_PREFIX ':'

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

#define MESSAGELINE1 " : **************************************************\n"
#define MESSAGELINE2 " : *             H    E    L    L    O              *\n"
#define MESSAGELINE3 " : *  This is a private irc server. Please contact  *\n"
#define MESSAGELINE4 " : *  the admin of the server for any questions or  *\n"
#define MESSAGELINE5 " : *  issues.                                       *\n"

#define GET_PASS_AUTH(auth) (auth) & AUTH::PASS
#define SWITCH_PASS_AUTH(auth) (auth) ^ AUTH::PASS

#define GET_USER_AUTH(auth) (auth) & AUTH::USER
#define SWITCH_USER_AUTH(auth) (auth) ^ AUTH::USER

#define GET_NICK_AUTH(auth) (auth) & AUTH::NICK
#define SWITCH_NICK_AUTH(auth) (auth) ^ AUTH::NICK

#define GET_SENT_AUTH(auth) (auth) & AUTH::SENT
#define SWITCH_SENT_AUTH(auth) (auth) ^ AUTH::SENT

#define BOT_NAME "ebangBot"

namespace BOT{
	enum CODE{
		FAIL = 0,
        CMD_NOT_FOUND = 1,
        GET_NOTICE = 8,
        SET_NOTICE = 12,
        GET_WELCOME = 10,
        SET_WELCOME = 15
	};
};

namespace PERMISSION{
	enum CODE{
		N = 1,	// 0000 0001
        I = 2,	// 0000 0010
        T = 4,	// 0000 0100
        K = 8,	// 0000 1000
        O = 16,	// 0001 0000
        L = 32	// 0010 0000
	};	
};

namespace AUTH{
	enum CODE{
		USER = 1,
		PASS = 2,
		SENT = 4,
		NICK = 8
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
		PING,
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
		UNKNOWN_CMD = 421,
		NO_PARAM = 696,
		NEED_MORE_PARAM = 461,
		ALREADY_REGISTERED = 462,
		PASS_MISMATCH = 464,
		CANNOTSENDTOCHAN = 404,

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
		INVITE = 341,

		RPL_NAMREPLY = 353,
		RPL_ENDOFNAMES = 366,
		TOPIC = 332,
		TOPIC_WHOTIME = 333,

		// --- server message ---
		WELCOME = 1,
		INTRO = 2,
		SERVERCREATE = 3,
		MYINFO = 4,
		CAPINFO = 5,
		USERINFO = 251,
		CLIENTINFO = 255,
		MESSAGESTART = 375,
		MESSAGEOFDAY = 372,
		MESSAGEEND = 376
	};
};

namespace CAP{
	enum  CODE{
		MODESIZE=6,
		CHANNELLEN=64,//  (채널 이름의 최대 길이)
		KEYLEN=32,//  	  (key 설정 시 최대 길이)
		KICKLEN=255,//   (KICK 시에 보내는 msg 최대길이)
		LINELEN=512,//   (buffer size)
		TOPICLEN=307,//  (TOPIC 최대 길이)
		MODES = 5, //(user가 /mode 명령어로 최대 설정할 수 있는 channel의 mode 수)
		USERLEN=10,//	(사용자 아이디의  최대  길이 (옥텟), number 지정 시 제한 없음)
		HOSTLEN=64, //   (client 의 host 이름 최대 길이. 너무 길게 되면 IP로 본다.)
		MAXTARGETS = 1,//(PRIVMSG 보내는 대상 지정 시 그 길이)	
		NICKLEN=30// (user nickname 길이)
	};
};

#endif