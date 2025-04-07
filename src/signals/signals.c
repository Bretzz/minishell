/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 18:07:19 by mapascal          #+#    #+#             */
/*   Updated: 2025/04/07 20:18:19 by topiana-         ###   ########.fr       */
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
void    input_initializer(void);
void    doc_initializer(void);

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
        g_last_sig = SIGINT;
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

		g_last_sig = SIGQUIT;
    }
}

static void input_handler(int signal)
{
    if (signal == SIGINT)
    {
        // Stampa un newline (così se sei in un comando come "cat", va a capo)
        write(STDOUT_FILENO, "\n", 1);    //heheh
        // Cancella la linea corrente e prepara readline per una nuova linea
   //     rl_replace_line("", 0);   //MacOS issues
    //	    rl_on_new_line();
        // rl_redisplay();
        g_last_sig = SIGINT;
    }
    else if (signal == SIGQUIT)
    {
		write(STDOUT_FILENO, "\n", 1);    //heheh
        // Cancella la linea corrente e prepara readline per una nuova linea
        rl_replace_line("", 0);   //MacOS issues
        //rl_on_new_line();
        rl_redisplay();
        //ft_printf("Pterodattilo!\n");
		g_last_sig = SIGQUIT;
    }
}

static void doc_handler(int signal)
{
    if (signal == SIGINT)
    {
        // Stampa un newline (così se sei in un comando come "cat", va a capo)
        write(STDOUT_FILENO, "\n", 1);    //heheh
       // ioctl(STDOUT_FILENO, I_FLUSH);
        // write(STDIN_FILENO, "\n", 1); 
        // Cancella la linea corrente e prepara readline per una nuova linea
        // rl_replace_line("", 0);   //MacOS issues
        // rl_on_new_line();
        // rl_redisplay();
        g_last_sig = SIGINT;
    }
    else if (signal == SIGQUIT)
    {
	//	rl_replace_line("", 0);   //MacOS issues
		write(STDERR_FILENO, "\b\b  \b\b", 6);
	//	rl_on_new_line();
     //   rl_redisplay();
        //ft_printf("Pterodattilo!\n");
		g_last_sig = SIGQUIT;
    }
}

void	idle_initializer(void)
{
	struct sigaction	waiter;

	bzero(&waiter, sizeof(waiter));
	waiter.sa_handler = idle_handler;
	waiter.sa_flags = 0;
	sigaction(SIGINT, &waiter, NULL);
	sigaction(SIGQUIT, &waiter, NULL);
}

void    input_initializer(void)
{
    struct sigaction	notebook;

	bzero(&notebook, sizeof(notebook));
	notebook.sa_handler = input_handler;
	notebook.sa_flags = SA_RESTART;
	sigaction(SIGINT, &notebook, NULL);
	sigaction(SIGQUIT, &notebook, NULL);
}

void    doc_initializer(void)
{
    struct sigaction	doc_here;

	bzero(&doc_here, sizeof(doc_here));
	doc_here.sa_handler = doc_handler;
	doc_here.sa_flags = 0;
	sigaction(SIGINT, &doc_here, NULL);
	sigaction(SIGQUIT, &doc_here, NULL);
}

// studiare con signal
