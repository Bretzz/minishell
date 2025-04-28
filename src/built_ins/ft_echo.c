/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 20:28:38 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/28 17:45:17 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "built_ins.h"

int	ft_echo(int *fd, t_cmd cmd);

static void	put_echo_words(int fd, char **words)
{
	int	i;

	i = 0;
	while (words[i] != NULL)
	{
		write(fd, words[i], ft_strlen(words[i]));
		if (words[i + 1] && words[i + 1][0] != '\0')
			write(fd, " ", 1);
		i++;
	}
}

static int	is_n_flag(char *arg)
{
	size_t	i;

	i = 0;
	if (arg[i++] != '-')
		return (0);
	while (arg[i] != '\0')
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

//echo     -n     ahahaha
/* TODO: handle PIPE and tell march that append is the O_FLAG fo read
(either O_WRITE | O_TRUNCATE or O_WRITE | O_APPEND ) */
int	ft_echo(int *fd, t_cmd cmd)
{
	unsigned int	i;

	if ((cmd.redir[1] == FILE && cmd.fd[1] < 0)
		|| (cmd.redir[0] == FILE && cmd.fd[0] < 0))
		return (safeclose(fd[1]), 1);
	if (cmd.words[1] && is_n_flag(cmd.words[1]))
	{
		i = 2;
		while (cmd.words[i] && is_n_flag(cmd.words[i]))
			i++;
		put_echo_words(fd[1], &cmd.words[i]);
		safeclose(fd[1]);
		return (0);
	}
	put_echo_words(fd[1], &cmd.words[1]);
	write(fd[1], "\n", 1);
	safeclose(fd[1]);
	return (0);
}
