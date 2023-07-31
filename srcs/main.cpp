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