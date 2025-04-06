/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 14:44:34 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/06 18:53:05 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "cmds.h"

int	here_doc(char *limiter, const char ***vars);

// static int	dummy_in_pipe(int *pipefd)
// {
// 	close(pipefd[0]);
// 	close(pipefd[1]);
// 	if (pipe(pipefd) < 0)
// 	{
// 		write(STDERR_FILENO, "minishell: pipe failure\n", 24);
// 		return (STDIN_FILENO);
// 		// ...or just return
// 	}
// 	close(pipefd[1]);
// 	return (pipefd[0]);
// }

/* Takes a string as parameter,
if the string is inquotes (ex: 'LIMITER', or "LIMITER")
removes the quotes and returs 0, otherwise returns 1. */
static int	strip_limiter(char *limiter)
{
	size_t	len;

	len = ft_strlen(limiter);
	if ((limiter[0] == '\'' || limiter[0] == '"') && limiter[0] == limiter[len - 1])
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
			write(fd, exp_line, ft_strlen(exp_line));
		free(exp_line);
	}
	else
	{
		write(fd, line, ft_strlen(line));
	}
	return (1);
}

/* Takes an string as parameters.
Fills a pipe with input from STDIN_FILENO until 'limiter' is found.
After that closes the write-end of the pipe and returns the read-end. */
int	here_doc(char *limiter, const char ***vars)
{
	int		pipefd[2];
	char	*line;
	char	exp_flag;
	int		i;

	if (pipe(pipefd) < 0)
	{
		write(STDERR_FILENO, "minishell: pipe failure\n", 24);
		return (STDIN_FILENO);
	}
	exp_flag = strip_limiter(limiter);
	doc_initializer();
	write(STDOUT_FILENO, "> ", 2);
	line = get_next_line(STDIN_FILENO);
	i = 1;
	while (/* line != NULL &&  */!(line && !ft_strncmp(limiter, line, ft_strlen(limiter))
			&& line[ft_strlen(limiter)] == '\n'))
	{
		if (line != NULL)
		{
			if (!exp_write(pipefd[1], exp_flag, line, vars))
			{
				free(line);
				idle_initializer();
				multicose(pipefd);
				return (dummy_in_pipe());
			}
			free(line);
		}
		else
		{
			if (g_last_sig == SIGINT)
			{
				idle_initializer();
				multicose(pipefd);
				return (dummy_in_pipe());
			}
			ft_printfd(STDERR_FILENO, "minishell: warning: here-document at line %i \
delimited by end-of-file (wanted `%s')\n", i, limiter);
			break ;
		}
		write(STDOUT_FILENO, "> ", 2);
		line = get_next_line(STDIN_FILENO);
		i++;
	}
	free(line);
	idle_initializer();
	close(pipefd[1]);
	return (pipefd[0]);
}
