/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 20:28:38 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/17 20:29:16 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_echo(t_cmd cmd, const char **shv, const char **env);

static void put_echo_words(int fd, char **words, const char **shv, const char **env)
{
	int i;
	char *exp_str;

	i = 0;

	while (words[i] && words[i][0] != '\0')
	{
		exp_str = expand_string(words[i], shv, env);
		ft_printfd(fd, "%s", exp_str);
		if (words[i+1] && words[i+1][0] != '\0')
			ft_printfd(fd, " ");
		free(exp_str);
		i++;
	}
}
//echo     -n     ahahaha
/* TODO: handle PIPE and tell march that append is the O_FLAG fo read
(either O_WRITE | O_TRUNCATE or O_WRITE | O_APPEND ) */
int	ft_echo(t_cmd cmd, const char **shv, const char **env)
{
	int	fd;

	if (cmd.words[1][0] == '\0')
		return (-1);
	
	fd = 1;
	if (!ft_strncmp("-n", cmd.words[1], 2))
	{
		put_echo_words(fd, &cmd.words[2], shv, env);
		return (1);
	}
	put_echo_words(fd, &cmd.words[1], shv, env);
	ft_printfd(fd, "\n");
	if (fd > 2)
		close(fd);
	return (1);
}
