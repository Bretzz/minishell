/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safety_first.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 00:03:12 by march             #+#    #+#             */
/*   Updated: 2025/04/11 18:11:33 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "cmds.h"

char	*safe_line(int fd);
int		doc_exp_write(int fd, char exp_flag, char *line, const char ***vars);

/* returns a line read from the 'fd'.
If a SIGQUIT or a SIGTSTP is intercepted tries again. */
char	*safe_line(int fd)
{
	char	*line;

	line = get_next_line(fd);
	while (!line && (g_last_sig == SIGQUIT || g_last_sig == SIGTSTP))
	{
		g_last_sig = 0;
		line = get_next_line(fd);
	}
	return (line);
}

/* takes the exp_flag, line to write and the fd as params.
If the flag is set to 0 the line is written as it is then freed, if not
the line is expanded. Then line is free'd.
RETURNS: 0 on malloc failure, 1 otherwise. */
int	doc_exp_write(int fd, char exp_flag, char *line, const char ***vars)
{
	char	*exp_line;

	if (exp_flag != 0)
	{
		exp_line = just_expand_string(line, (const char ***)vars);
		if (exp_line == NULL)
		{
			write(STDOUT_FILENO, "minishell malloc failure\n", 26);
			return (0);
		}
		else
		{
			write(fd, exp_line, ft_strlen(exp_line));
		}
		free(exp_line);
	}
	else
	{
		write(fd, line, ft_strlen(line));
	}
	return (1);
}
