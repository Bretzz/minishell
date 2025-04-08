/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totommi <totommi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 18:07:19 by mapascal          #+#    #+#             */
/*   Updated: 2025/04/08 01:20:56 by totommi          ###   ########.fr       */
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
void    runtime_initializer(void);
void    input_initializer(void);

static void idle_handler(int signal)
{
    if (signal == SIGINT)
    {
        // Stampa un newline (così se sei in un comando come "cat", va a capo)
        write(STDOUT_FILENO, "\n", 1);    //heheh
        // Cancella la linea corrente e prepara readline per una nuova linea
        rl_replace_line("", 0);   //MacOS issues
        rl_on_new_line();
        rl_redisplay();
    }
    else if (signal == SIGQUIT)
    {
	//	write(STDOUT_FILENO, "\n", 1);    //heheh
	//	rl_replace_line("Pterodattilo!\n", 14);
		write(STDERR_FILENO, "\b\b  \b\b", 6);    //heheh
        //ft_printf("Pterodattilo!\n");
		// rl_replace_line("", 0);   //MacOS issues
        // //rl_on_new_line();
        // rl_redisplay();
    }
	g_last_sig = signal;
}

static void runtime_handler(int signal)
{
    if (signal == SIGINT)
    {
        // Stampa un newline (così se sei in un comando come "cat", va a capo)
        write(STDOUT_FILENO, "\n", 1);    //heheh
        // Cancella la linea corrente e prepara readline per una nuova linea
   //     rl_replace_line("", 0);   //MacOS issues
    //	    rl_on_new_line();
        // rl_redisplay();
    }
    else if (signal == SIGQUIT)
    {
		write(STDOUT_FILENO, "(Quit)Core bumped\n", 19);    //heheh
        // Cancella la linea corrente e prepara readline per una nuova linea
        rl_replace_line("", 0);   //MacOS issues
        //rl_on_new_line();
        rl_redisplay();
        //ft_printf("Pterodattilo!\n");
    }
	g_last_sig = signal;
}

static void input_handler(int signal)
{
    if (signal == SIGINT)
    {
        // Stampa un newline (così se sei in un comando come "cat", va a capo)
        write(STDOUT_FILENO, "\n", 1);    //heheh
    }
    else if (signal == SIGQUIT || signal == SIGTSTP)
    {
		write(STDERR_FILENO, "\b\b  \b\b", 6);
    }
	g_last_sig = signal;
}

/* Handle signals while waiting for a new command line. */
void	idle_initializer(void)
{
	struct sigaction	waiter;

	bzero(&waiter, sizeof(waiter));
	waiter.sa_handler = idle_handler;
	waiter.sa_flags = 0;
	sigaction(SIGINT, &waiter, NULL);
	sigaction(SIGQUIT, &waiter, NULL);
	//rl_catch_signals = 0;
}

/* Handle signals during the execution of other commands. */
void    runtime_initializer(void)
{
    struct sigaction	fire_fighter;

	bzero(&fire_fighter, sizeof(fire_fighter));
	fire_fighter.sa_handler = runtime_handler;
	fire_fighter.sa_flags = SA_RESTART;
	sigaction(SIGINT, &fire_fighter, NULL);
	sigaction(SIGQUIT, &fire_fighter, NULL);
}

/* Handle signals while the user is providing
an input, ex: here-document, unclosed quotes, etc.. */
void    input_initializer(void)
{
    struct sigaction	input_guard;

	bzero(&input_guard, sizeof(input_guard));
	input_guard.sa_handler = input_handler;
	input_guard.sa_flags = 0;
	sigaction(SIGINT, &input_guard, NULL);
	sigaction(SIGQUIT, &input_guard, NULL);
	sigaction(SIGTSTP, &input_guard, NULL);
}

// studiare con signal
