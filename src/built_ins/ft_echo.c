/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 20:28:38 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/18 21:12:10 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_echo(int *fd, t_cmd cmd, const char **shv, const char **env);

static void put_echo_words(int *fd, char **words, const char **shv, const char **env)
{
	int i;
	char *exp_str;

	i = 0;

	(void)fd;
	while (words[i] && words[i][0] != '\0')
	{
		exp_str = expand_string(words[i], shv, env);
		write(fd[1], exp_str, ft_strlen(exp_str));
		if (words[i+1] && words[i+1][0] != '\0')
			write(fd[1], " ", 1);
		free(exp_str);
		i++;
	}
}
//echo     -n     ahahaha
/* TODO: handle PIPE and tell march that append is the O_FLAG fo read
(either O_WRITE | O_TRUNCATE or O_WRITE | O_APPEND ) */
int	ft_echo(int *fd, t_cmd cmd, const char **shv, const char **env)
{
	if (cmd.words[1][0] == '\0') //should never happen
		return (safeclose(fd[1]), 0);
	
	if (!ft_strncmp("-n", cmd.words[1], 2))
	{
		put_echo_words(fd, &cmd.words[2], shv, env);
		safeclose(fd[1]);
		return (0);
	}
	put_echo_words(fd, &cmd.words[1], shv, env);
	write(fd[1], "\n", 1);
	safeclose(fd[1]);
	return (0);
}
