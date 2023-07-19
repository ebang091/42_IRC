CC = c++
CXXFLAGS = -std=c++98 -fsanitize=address #-Wall -Wextra -Werror
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

.PHONY : all, clean, fclean, re