/*
	1. ClassName : UpperCamelCase
	2. MemberFunctions : lowerCamelCase
	3. MemberVariables : lowerCamelCase && use _
	4. use Exception
*/

/*
	TODO List
	0. 서버 동작 확인
	1. 채널, 클라이언트 클래스 설계
		1. 클라 : 클라 클래스 만들기, 매니저에서 두 가지의 map getter, setter 구현
		2. 채널 : 권한 getter setter 함수
	2. 파싱
	3. 커맨드 처리
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