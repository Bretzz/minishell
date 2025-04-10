/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: march <march@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 14:44:34 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/11 00:09:03 by march            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "cmds.h"

int		here_doc(char *limiter, const char ***vars);
char	*safe_line(int fd);

/* Takes a string as parameter,
if the string is inquotes (ex: 'LIMITER', or "LIMITER")
removes the quotes and returs 0, otherwise returns 1. */
static int	strip_limiter(char *limiter)
{
	size_t	len;

	len = ft_strlen(limiter);
	if ((limiter[0] == '\'' || limiter[0] == '"')
		&& limiter[0] == limiter[len - 1])
	{
		limiter[len - 1] = '\0';
		ft_memmove(limiter, (limiter + 1), len);
		return (0);
	}
	return (1);
}

/* takes the exp_flag, line to write and the fd as params.
If the flag is set to 0 the line is written as it is then freed, if not
the line is expanded. Then line is free'd.
RETURNS: 0 on malloc failure, 1 otherwise. */
static int	exp_write(int fd, char exp_flag, char *line, const char ***vars)
{
	char	*exp_line;

	if (exp_flag != 0)
	{
		exp_line = just_expand_string(line, vars);
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

/* this function handle eventual signals
that interrupted the heredoc creation.
RETURNS: the fd to read from. */
static int	null_catcher(int fd, int line_count, char *limiter)
{
	if (g_last_sig == SIGINT)
	{
		g_last_sig = 0;
		clean_exit(fd);
		return (safeclose(open_doc(CREATE)),
			read_doc(open_doc(GETNUM)));
	}
	else if (g_last_sig == SIGQUIT || g_last_sig == SIGTSTP)
		g_last_sig = 0;
	else
	{
		ft_printfd(STDERR_FILENO, "minishell: warning: here-document at line %i \
delimited by end-of-file (wanted `%s')\n", line_count, limiter);
	}
	clean_exit(fd);
	return (safeclose(open_doc(CREATE)),
		read_doc(open_doc(GETNUM)));
}

/* Reads from STDIN until the limiter is found.
Writes each line to the given fd, expanding variables if needed.
Returns -1 on success, or the read-end fd on early exit. */
int	read_until_limiter(int fd, char *lim, char exp_flag, const char ***vars)
{
	char	*line;
	int		i;

	line = safe_line(STDIN_FILENO);
	i = 1;
	while (line != NULL && !(line && !ft_strncmp(lim, line, ft_strlen(lim))
			&& line[ft_strlen(lim)] == '\n'))
	{
		if (!exp_write(fd, exp_flag, line, vars))
		{
			free(line);
			clean_exit(fd);
			return (safeclose(open_doc(CREATE)),
				read_doc(open_doc(GETNUM)));
		}
		free(line);
		write(STDOUT_FILENO, "> ", 2);
		line = safe_line(STDIN_FILENO);
		i++;
	}
	if (line == NULL)
		return (null_catcher(fd, i, lim));
	free(line);
	return (-1);
}

/* Takes an string as parameters.
Fills a pipe with input from STDIN_FILENO until 'limiter' is found.
After that closes the write-end of the pipe and returns the read-end. */
int	here_doc(char *lim, const char ***vars)
{
	int			fd;
	char		exp_flag;

	fd = open_doc(CREATE);
	if (fd < 0)
		return (STDIN_FILENO);
	exp_flag = strip_limiter(lim);
	input_initializer();
	write(STDOUT_FILENO, "> ", 2);
	line = safe_line(STDIN_FILENO);
	i = 1;
	if (read_until_limiter(fd, lim, exp_flag, vars) != -1)
		return (fd);
	if (line == NULL)
		return (null_catcher(fd, i, lim));
	free(line);
	safeclose(fd);
	fd = read_doc(open_doc(GETNUM));
	if (fd < 0)
		return (STDIN_FILENO);
	return (fd);
}
