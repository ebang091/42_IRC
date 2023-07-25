# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ebang <ebang@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/13 19:22:49 by kwsong            #+#    #+#              #
#    Updated: 2023/07/25 21:44:54 by ebang            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# make all - make debug - make client1, make client2 ... 

all 		:
	docker build . --tag irc:my
	docker run --name irc --rm irc:my

run		:
	docker run --name irc --rm irc:my


c1		:
	docker exec -it irc irssi -c 127.0.0.1 -p 6667 -n one -w 1234

c2		:
	docker exec -it irc irssi -c 127.0.0.1 -p 6667 -n two

c3		:
	docker exec -it irc irssi -c 127.0.0.1 -p 6667 -n three

c4		:
	docker exec -it irc irssi -c 127.0.0.1 -p 6667 -n four

c5		:
	docker exec -it irc irssi -c 127.0.0.1 -p 6667 -n five

n1		:
	docker exec -it irc nc 127.0.0.1 6667

n2		:
	docker exec -it irc nc 127.0.0.1 6667

n3		:
	docker exec -it irc nc 127.0.0.1 -p 6667

n4		:
	docker exec -it irc nc 127.0.0.1 -p 6667

n5		:
	docker exec -it irc nc 127.0.0.1 -p 6667


exec		:
	docker exec -it irc bash

down		:
	docker stop irc

clean		:
	docker system prune -a

re			:
	make clean
	make all

.PHONY		: all run c1 c2 c3 c4 c5 exec down clean re
