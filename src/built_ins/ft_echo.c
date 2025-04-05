/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totommi <totommi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 20:28:38 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/05 18:16:24 by totommi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_echo(int *fd, t_cmd cmd);

static void put_echo_words(int fd, char **words)
{
	int i;
	
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
	// if (cmd.words[1] == NULL) //should never happen
	// 	return (safeclose(fd[1]), 0);
	
	if (cmd.words[1] && !ft_strncmp("-n", cmd.words[1], 2))
	{
		put_echo_words(fd[1], &cmd.words[2]);
		safeclose(fd[1]);
		return (0);
	}
	put_echo_words(fd[1], &cmd.words[1]);
	write(fd[1], "\n", 1);
	safeclose(fd[1]);
	return (0);
}
