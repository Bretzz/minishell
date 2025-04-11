/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 00:18:23 by march             #+#    #+#             */
/*   Updated: 2025/04/11 15:59:50 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "signals.h"

void	idle_initializer(void);
void	runtime_initializer(void);
void	input_initializer(void);
void	ft_readline_initializer(void);

/* Handle signals while waiting for a new command line. */
void	idle_initializer(void)
{
	struct sigaction	waiter;

	bzero(&waiter, sizeof(waiter));
	waiter.sa_handler = idle_handler;
	waiter.sa_flags = 0;
	sigaction(SIGINT, &waiter, NULL);
	sigaction(SIGQUIT, &waiter, NULL);
	sigaction(SIGTSTP, &waiter, NULL);
}

/* Handle signals during the execution of other commands. */
void	runtime_initializer(void)
{
	struct sigaction	fire_fighter;

	bzero(&fire_fighter, sizeof(fire_fighter));
	fire_fighter.sa_handler = runtime_handler;
	fire_fighter.sa_flags = SA_RESTART;
	sigaction(SIGINT, &fire_fighter, NULL);
	sigaction(SIGQUIT, &fire_fighter, NULL);
	sigaction(SIGTSTP, &fire_fighter, NULL);
}

/* Handle signals while the user is providing
an input, ex: here-document, unclosed quotes, etc.. */
void	input_initializer(void)
{
	struct sigaction	input_guard;

	bzero(&input_guard, sizeof(input_guard));
	input_guard.sa_handler = input_handler;
	input_guard.sa_flags = 0;
	sigaction(SIGINT, &input_guard, NULL);
	sigaction(SIGQUIT, &input_guard, NULL);
	sigaction(SIGTSTP, &input_guard, NULL);
}

void	ft_readline_initializer(void)
{
	struct sigaction	come_and_get_me;

	bzero(&come_and_get_me, sizeof(come_and_get_me));
	come_and_get_me.sa_handler = ft_readline_handler;
	come_and_get_me.sa_flags = 0;
	sigaction(SIGINT, &come_and_get_me, NULL);
	sigaction(SIGQUIT, &come_and_get_me, NULL);
}
