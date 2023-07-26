CC = c++
CXXFLAGS = -std=c++98  -Wall -Wextra -Werror #-fsanitize=address -g
SRCS = main.cpp \
		Server.cpp \
		Parser.cpp \
		SocketHandler.cpp \
		ErrorHandler.cpp \
		EventHandler.cpp \
		CommandHandler.cpp \
		MessageHandler.cpp \
		ClientManager.cpp \
		ChannelManager.cpp \
		Client.cpp \
		Channel.cpp \
		FSM.cpp
OBJS = $(SRCS:.cpp=.o)
NAME = ircserv

all : $(NAME)

$(NAME) : $(OBJS)
	$(CC) $(CXXFLAGS) -o $@ $^

%.o : %.cpp
	$(CC) $(CXXFLAGS) -o $@ -c $^

clean :
	rm  -rf $(OBJS)

fclean :
	$(MAKE) clean
	rm -rf $(NAME)

re :
	$(MAKE) fclean
	$(MAKE) all

n1		:
	nc 127.0.0.1 4242

n2		:
	nc 127.0.0.1 4242

n3		:
	nc 127.0.0.1 4242

debug :
	make re -j8
	./ircserv 4242 1234
	irssi -c 127.0.0.1 -p 4242

.PHONY : all, clean, fclean, re, debug, n1, n2, n3