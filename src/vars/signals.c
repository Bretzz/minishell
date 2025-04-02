/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 18:07:19 by mapascal          #+#    #+#             */
/*   Updated: 2025/04/02 17:51:03 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <string.h>
#include <sys/ioctl.h>

static void sig_handler(int signal)
{
    if (signal == SIGINT)
    {
        // Stampa un newline (così se sei in un comando come "cat", va a capo)
        write(STDOUT_FILENO, "\n", 1);
        // Cancella la linea corrente e prepara readline per una nuova linea
        rl_replace_line("", 0);
        rl_on_new_line();
        rl_redisplay();
        g_last_sig = 130;
    }
    else if (signal == SIGQUIT)
    {
        ft_printf("Pterodattilo!\n");
    }
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
