/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: march <march@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 18:07:19 by mapascal          #+#    #+#             */
/*   Updated: 2025/04/11 00:18:09 by march            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <string.h>
#include <sys/ioctl.h>

void	idle_initializer(void);
void	runtime_initializer(void);
void	input_initializer(void);
void	ft_readline_initializer(void);

static void	e_handler(int signl)
{
	if (signal == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	else if (signal == SIGQUIT)
	{
		write(STDERR_FILENO, "\b\b  \b\b", 6);
	}
	g_last_sig = signal;
}

static void	time_handler(int signl)
{
	if (signal == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
	}
	else if (signal == SIGQUIT)
	{
		write(STDOUT_FILENO, "(Quit)Core bumped\n", 19);
		rl_replace_line("", 0);
		rl_redisplay();
	}
	g_last_sig = signal;
}

static void	ut_handler(int signl)
{
	if (signal == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
	}
	else if (signal == SIGQUIT || signal == SIGTSTP)
	{
		write(STDERR_FILENO, "\b\b  \b\b", 6);
	}
	g_last_sig = signal;
}

static void	ft_readline_handler(int signal)
{
	if (signal == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	else if (signal == SIGQUIT)
	{
		write(STDERR_FILENO, "\b\b  \b\b", 6);
	}
	g_last_sig = signal;
}
