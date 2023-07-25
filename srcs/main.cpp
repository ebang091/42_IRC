/*
	1. ClassName : UpperCamelCase
	2. MemberFunctions : lowerCamelCase
	3. MemberVariables : lowerCamelCase && use _
	4. use Exception
*/
/*
	TODO
	1. Send Error to Exception
	2. command message 처리
	3. PING, CAP LS
	4. 채팅방에 들어와있는 상태에서 닉네임을 바꾸면 모든 채팅방에서 그 클라이언트를 지우고 다시 새로운 닉네임으로 추가함
	5. 버퍼에 담고 다음에 전송됨
	4. 이벤트 핸들러 메인 함수 리팩토링 (모듈화 더)

	join, part channel list 순회하면서 에러 발생 시 해당 channel만 실패하고 계속

 	AWAYLEN=200 CASEMAPPING=rfc1459 CHANLIMIT=#:20 CHANMODES=b,k,l,imnpst CHANNELLEN=64 CHANTYPES=# ELIST=CMNTU HOSTLEN=64 KEYLEN=32 KICKLEN=255 LINELEN=512 MAXLIST=b:100 :are supported by this server
 	MAXTARGETS=20 MODES=20 NAMELEN=128 NETWORK=Localnet NICKLEN=30 PREFIX=(ov)@+ SAFELIST STATUSMSG=@+ TOPICLEN=307 USERLEN=10 USERMODES=,,s,iow WHOX :are supported by this server

	NETWORK=Localnet
	
	제한 사항
	**channel**

	CHANLIMIT=#:20 (유저가 참여할 수 있는 최대 채널 수)
	CHANMODES=k,l,o,int  (사용 가능한 채널 모드)
	CHANNELLEN=64  (채널 이름의 최대 길이)
	CHANTYPES=#   ('#' 모든 서버가 알고 있다는 의미.)
	KEYLEN=32  	  (key 설정 시 최대 길이)
	KICKLEN=255   (KICK 시에 보내는 msg 최대길이)
	LINELEN=512   (buffer size)
	TOPICLEN=307  (TOPIC 최대 길이)
	MODES (user가 /mode 명령어로 최대 설정할 수 있는 channel의 mode 수)

	**user**

	USERLEN=10	(사용자 아이디의  최대  길이 (옥텟), number 지정 시 제한 없음)
	HOSTLEN=64    (client 의 host 이름 최대 길이. 너무 길게 되면 IP로 본다.)
	MAXTARGETS (PRIVMSG 보내는 대상 지정 시 그 길이)	
	NAMELEN=128 (user name 길이)
	NICKLEN=30 (user nickname 길이)
	PREFIX=@ (채널 특권을 표현하는 문자. @: operator && creator)
	*/

#include <iostream>
#include "Server.hpp"
#include "Parser.hpp"

int main(int argc, char **argv){
	if(argc != 3){
		std::cout << "usage: ./isrcserv <port> <password>\n";
		return 1;
	}

	Server::getInstance().run(argv[1], argv[2]);

	//test : NICK 으로 닉네임 만든 후 JOIN 해서 채널에 등록되는지 확인  : getClientByNick 해보자
	return 0;
}