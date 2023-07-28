# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ebang <ebang@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/13 19:22:49 by kwsong            #+#    #+#              #
#    Updated: 2023/07/28 16:24:47 by ebang            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv

CC = c++
CXXFLAGS = -std=c++98 -Wall -Wextra -Werror -fsanitize=address -g

SRCS_FILE = main.cpp \
		ErrorHandler.cpp \
		Server/Server.cpp \
		Server/Parser.cpp \
		Server/SocketHandler.cpp \
		Server/EventHandler.cpp \
		Command/CommandHandler.cpp \
		Command/MessageHandler.cpp \
		Command/FSM.cpp \
		Channel/ChannelManager.cpp \
		Channel/Channel.cpp \
		Client/ClientManager.cpp \
		Client/Client.cpp \
		Bot/Bot.cpp
OBJS_FILE = $(SRCS_FILE:.cpp=.o)

SRCS_DIR = ./srcs/
INCS_DIR = ./includes/

SRCS = $(addprefix $(SRCS_DIR), $(SRCS_FILE))
OBJS = $(addprefix $(SRCS_DIR), $(OBJS_FILE))

all 	: $(NAME)

$(NAME)	: $(OBJS)
	$(CC) $(CXXFLAGS) -o $@ $^ -I$(INCS_DIR)

%.o		: %.cpp
	$(CC) $(CXXFLAGS) -o $@ -c $< -I$(INCS_DIR)
	
clean	:
	rm  -rf $(OBJS)

fclean	:
	$(MAKE) clean
	rm -rf $(NAME)

re		:
	$(MAKE) fclean
	$(MAKE) all

n1		:
	nc 127.0.0.1 4242

n2		:
	nc 127.0.0.1 4242

n3		:
	nc 127.0.0.1 4242

c1		:
	irssi -c 127.0.0.1 -p 4242 -n one -w 1234

c2		:
	irssi -c 127.0.0.1 -p 4242 -n two -w 1234

c3		:
	irssi -c 127.0.0.1 -p 4242 -n three -w 1234


.PHONY	: all clean fclean re debug n1 n2 n3 c1 c2 c3
