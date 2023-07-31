/*

	
	1. 이벤트 핸들러 메인 함수 리팩토링 (모듈화 더


 		1. buffer still exist after quit
		2. privmsg to user (check return value of getDescription)
		3. join() 167 line -> if (!keyList.empty()) keyList.pop()
		4. empty key list -> bad channel key?? (msg)
		5. join with empty parameter
	6. server down when fail to send
		7. privmsg to unauthed user ()
		8. pop before check k permission at minusK
		9. change nickname to Ctrl+G
		10. display current mode on channel
		11. topic set include clone
		12. erase client when EV_EOF on
		13. FSM line 33, NULL guard
		14. non printable nickname

	* stress test

	* send line at once (mode)

	*/

#include <iostream>
#include "Server.hpp"

int main(int argc, char **argv){
	if(argc != 3){
		std::cout << "usage: ./isrcserv <port> <password>\n";
		return 1;
	}

	Server::getInstance().run(argv[1], argv[2]);
	return 0;
}