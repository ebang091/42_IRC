/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebang <ebang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 14:47:13 by kwsong            #+#    #+#             */
/*   Updated: 2023/07/14 12:21:01 by ebang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>

#define NELEMS(x) (sizeof(x) / sizeof((x)[0]))
// -------------------------------------------------------------------------- //
//                                    Colors                                  //
// -------------------------------------------------------------------------- //
# define ANSI_COLOR_RED      "\x1b[31m"                                       //  
# define ANSI_COLOR_GREEN    "\x1b[32m"                                       //
# define ANSI_COLOR_YELLOW   "\x1b[33m"                                       //
# define ANSI_COLOR_BLUE     "\x1b[34m"                                       //
# define ANSI_COLOR_MAGENTA  "\x1b[35m"                                       //
# define ANSI_COLOR_CYAN     "\x1b[36m"                                       //
# define ANSI_COLOR_RESET    "\x1b[0m"                                        //
// -------------------------------------------------------------------------- //

#include <string.h>
int main(int ac, char **av)
{
	if (ac != 2)
	{
		printf("Error : must need 1 argument [seconds]\n");
		return 0;
	}

	int num = atoi(av[1]);

	for (int i=0; i<num; ++i)
	{
		system("clear");
		printf(ANSI_COLOR_CYAN "%d\n", num - i);
		usleep(1000 * 1000);
	}

	for (int i=0; i<20; ++i)
		printf(ANSI_COLOR_RED "0000000000000000000000\n" ANSI_COLOR_RESET);
}