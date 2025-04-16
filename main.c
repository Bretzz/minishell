/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 16:05:45 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/16 17:00:12 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_last_sig = 0;

static char	*get_my_line_pls(char *prompt, int sigflag)
{
	if (sigflag == 1)
	{
		idle_initializer();
		return (readline(prompt));
	}
	else
		return (trim_back_nl(get_next_line(STDIN_FILENO)));
}

int	main(int argc, char *argv[], char *__environ[])
{
	char	*line;
	char	**vars[2];
	char	prompt[MAX_PATH + 10];

	vars_init(__environ, vars);
	stdin_init(argc, argv, vars);
	sigflag_init(vars);
	ft_bzero(prompt, sizeof(prompt));
	if (mtx_findval("USER", prompt, sizeof(prompt), vars[1]) == NULL
		&& mtx_findval("LOGNAME", prompt, sizeof(prompt), vars[1]) == NULL)
		ft_strlcpy(prompt, "pterodattilo!$_", sizeof(prompt));
	ft_strlcat(prompt, "$ ", sizeof(prompt));
	while (1)
	{
		line = get_my_line_pls(prompt, *((unsigned char *)vars[0] + 6));
		handle_line(&line, vars);
		free(line);
		close_docs();
	}
	return (*((unsigned char *)vars[0] + 5));
}
