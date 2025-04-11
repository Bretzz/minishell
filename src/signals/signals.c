/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 18:07:19 by mapascal          #+#    #+#             */
/*   Updated: 2025/04/11 16:00:50 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "signals.h"

void	idle_handler(int signum);
void	runtime_handler(int signum);
void	input_handler(int signum);
void	ft_readline_handler(int signum);

void	idle_handler(int signum)
{
	if (signum == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	else if (signum == SIGTSTP)
	{
		write(STDERR_FILENO, "\b\b  \b\b", 6);
	}
	else if (signum == SIGQUIT)
	{
		write(STDERR_FILENO, "\b\b  \b\b", 6);
	}
	g_last_sig = signum;
}

void	runtime_handler(int signum)
{
	if (signum == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
	}
	else if (signum == SIGQUIT)
	{
		write(STDOUT_FILENO, "(Quit)Core bumped\n", 19); //fix this
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (signum == SIGTSTP)
	{
		rl_replace_line("", 0);
		rl_redisplay();
	}
	g_last_sig = signum;
}

void	input_handler(int signum)
{
	if (signum == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
	}
	else if (signum == SIGQUIT || signum == SIGTSTP)
	{
		write(STDERR_FILENO, "\b\b  \b\b", 6);
	}
	g_last_sig = signum;
}

void	ft_readline_handler(int signum)
{
	if (signum == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	else if (signum == SIGQUIT)
	{
		write(STDERR_FILENO, "\b\b  \b\b", 6);
	}
	g_last_sig = signum;
}
