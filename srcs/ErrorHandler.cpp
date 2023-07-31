#include "ErrorHandler.hpp"

const char* ErrorHandler::PortNmberException::what() const throw()
{
	return "Error : Invalid port number";
}

const char* ErrorHandler::SocketException::what() const throw()
{
	return "Error : failed socket()";
}

const char* ErrorHandler::BindException::what() const throw()
{
	return "Error : failed bind()";
}

const char* ErrorHandler::ListenException::what() const throw()
{
	return "Error : failed listen()";
}

const char* ErrorHandler::AcceptException::what() const throw()
{
	return "Error : failed accept()";
}

const char* ErrorHandler::FcntlException::what() const throw()
{
	return "Error : failed fcntl()";
}

const char* ErrorHandler::KqueueException::what() const throw()
{
	return "Error : failed kqueue()";
}

const char* ErrorHandler::KeventException::what() const throw()
{
	return "Error : failed kevent()";
}

const char* ErrorHandler::QuitException::what() const throw()
{
	return "Quit User";
}