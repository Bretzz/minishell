/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 14:44:34 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/12 18:27:29 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "cmds.h"

int		here_doc(char *limiter, const char ***vars);

static void	clean_exit(int fd)
{
	char	path[MAX_PATH];

	safeclose(fd);
	unlink(get_doc_path(open_doc(GETNUM), path, sizeof(path)));
}

/* this function handle eventual signals
that interrupted the heredoc creation.
RETURNS: the fd to read from, -1 if SIGINT is cought. */
static int	null_catcher(int fd, int line_count, char *limiter)
{
	if (g_last_sig == SIGINT)
	{
		clean_exit(fd);
		return (-1);
	}
	else if (g_last_sig != SIGQUIT && g_last_sig != SIGTSTP)
	{
		ft_printfd(STDERR_FILENO, "minishell: warning: here-document at line %i \
delimited by end-of-file (wanted `%s')\n", line_count, limiter);
	}
	return (safeclose(fd), read_doc(open_doc(GETNUM)));
}

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

/* Reads from STDIN until the limiter is found.
Writes each line to the given fd, expanding variables if needed.
Returns -1 on success, or the read-end fd on early exit. */
int	read_until_limiter(int fd, char *lim, char exp_flag, const char ***vars)
{
	char	*line;
	int		i;

	line = safe_line("> ", *((unsigned char *)vars[0] + 6));
	i = 1;
	while (line != NULL && !(line && !ft_strncmp(lim, line, ft_strlen(lim))
			&& line[ft_strlen(lim)] == '\n'))
	{
		if (!doc_exp_write(fd, exp_flag, line, vars))
		{
			free(line);
			clean_exit(fd);
			return (safeclose(open_doc(CREATE)),
				read_doc(open_doc(GETNUM)));
		}
		free(line);
		line = safe_line("> ", *((unsigned char *)vars[0] + 6));
		i++;
	}
	if (line == NULL)
		return (null_catcher(fd, i, lim));
	free(line);
	return (safeclose(fd), read_doc(open_doc(GETNUM)));
}

/* Takes an string as parameters.
Fills a pipe with input from STDIN_FILENO until 'limiter' is found.
After that closes the write-end of the pipe and returns the read-end.
NOTE: -1 is returned if SIGINT is cought. */
int	here_doc(char *lim, const char ***vars)
{
	int				fd;
	char			exp_flag;

	exp_flag = strip_limiter(lim);
	if (*((unsigned char *)vars[0] + 6) == 1)
		input_initializer();
	fd = read_until_limiter(open_doc(CREATE), lim, exp_flag, vars);
	if (*((unsigned char *)vars[0] + 6) == 1)
		idle_initializer();
	return (fd);
}
