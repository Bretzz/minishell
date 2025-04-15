/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 20:28:38 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/15 20:28:06 by topiana-         ###   ########.fr       */
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

//echo     -n     ahahaha
/* TODO: handle PIPE and tell march that append is the O_FLAG fo read
(either O_WRITE | O_TRUNCATE or O_WRITE | O_APPEND ) */
int	ft_echo(int *fd, t_cmd cmd)
{
	unsigned int	i;

	if (cmd.words[1] && !ft_strncmp("-n", cmd.words[1], 2))
	{
		i = 2;
		while (cmd.words[i] && !ft_strncmp("-n", cmd.words[i], 2))
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
