/*
	1. ClassName : UpperCamelCase
	2. MemberFunctions : lowerCamelCase
	3. MemberVariables : lowerCamelCase && use _
	4. use Exception
*/

/*
	TODO List
	1. 서버 동작 확인
	2. 채널, 클라이언트 클래스 설계 (채널 매니저, 클라이언트 매니저)
*/

#include <iostream>
#include "Server.hpp"

int main(int argc, char **argv){
	if(argc != 3){
		std::cout << "usage: ./isrcserv <port> <password>\n";
		return 1;
	}

	Server::getInstance().run(argv[1], argv[2]);
}