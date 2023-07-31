#pragma once
#ifndef ERROR_HANDLER_HPP
#define ERROR_HANDLER_HPP

#include <exception>
#include <unistd.h>

class ErrorHandler
{
private:
	ErrorHandler(){};

public:
	class PortNmberException : public std::exception{
	public:
		const char* what() const throw();
	};

    class SocketException : public std::exception{
	public:
		const char* what() const throw();
	};
	
	class BindException : public std::exception{
	public:
		const char* what() const throw();
	};

    class ListenException : public std::exception{
	public:
		const char* what() const throw();
	};

    class AcceptException : public std::exception{
	public:
		const char* what() const throw();
	};

	class FcntlException : public std::exception{
	public:
		const char* what() const throw();
	};

	class KqueueException : public std::exception{
	public:
		const char* what() const throw();
	};

	class KeventException : public std::exception{
	public:
		const char* what() const throw();
	};
	
	class QuitException : public std::exception{
	public:
		const char* what() const throw();
	};
};

#endif