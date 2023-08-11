#include <iostream>
#include "Server.hpp"

// void checkLeak()
// {
// 	system("leaks ircserv");
// }

int main(int argc, char **argv){
	//atexit(checkLeak);
	if(argc != 3){
		std::cout << "usage: ./isrcserv <port> <password>\n";
		return 1;
	}

	try{
		Server::getInstance().run(argv[1], argv[2]);
	}
    catch (const std::exception& e){
		std::cout << e.what() << std::endl;
		return 1;
    }
	return 0;
}