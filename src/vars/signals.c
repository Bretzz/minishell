/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapascal <mapascal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 18:07:19 by mapascal          #+#    #+#             */
/*   Updated: 2025/03/21 19:47:57 by mapascal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <string.h>
#include <sys/ioctl.h>

void	sig_handler(int signal)
{
	if(signal == SIGINT)
	{		
		ioctl(STDIN_FILENO, TIOCSTI, "\n");
		g_last_sig = 130;

	}
	if(signal == SIGQUIT)
		printf("Pterodattilo!\n");
}

void	sig_initializer(void)
{
	struct sigaction	messanger;

	bzero(&messanger, sizeof(messanger));
	messanger.sa_handler = sig_handler;
	messanger.sa_flags = SA_RESTART;
	sigaction(SIGINT, &messanger, NULL);
	sigaction(SIGQUIT, &messanger, NULL);
}

// studiare con signal
