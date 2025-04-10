/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_cute.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: march <march@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 13:46:21 by totommi           #+#    #+#             */
/*   Updated: 2025/04/10 18:02:18 by march            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "cmds.h"

int		dummy_in_pipe(void);
t_cmd	pop_arg(t_cmd cmd, int index);
int		ft_wifexited(pid_t pid);
int		exec_external(int *execfd, char *line, t_cmd *cmd, char ***vars);
int		is_directory(const char *path);

int	is_directory(const char *path)
{
	DIR	*dir;

	dir = opendir(path);
	if (dir == NULL)
		return (0);
	closedir(dir);
	return (1);
}

/* creates a pipe and returns the read-end of it
after closing the write-end.
RETURNS: the read-end [0] of the pipe,
STDIN_FILENO on pipe(3). */
int	dummy_in_pipe(void)
{
	int	pipefd[2];

	pipefd[0] = STDIN_FILENO;
	pipefd[1] = STDOUT_FILENO;
	if (pipe(pipefd) < 0)
	{
		write(STDERR_FILENO, "minishell: pipe failure\n", 24);
		return (STDIN_FILENO);
	}
	safeclose(pipefd[1]);
	return (pipefd[0]);
}

/* takes a cmd struct and an intager as parameter.
Then removes (read below) the corresponding string of the
'words' attribute of the cmd, if index is less than 0
or there are less not NULL pointer in 'words'
than the index returns the passed struct, otherwise
return the struct with the index'd word removed.
By 'removes' I mean:
	1. free(3) the area pointed by the word,
	2. set the word slot tu NULL,
	3. bring up all the pointers below that
		so that there are no NULL between words. */
t_cmd	pop_arg(t_cmd cmd, int index)
{
	size_t			len;
	unsigned int	i;

	len = ft_mtxlen((const void **)cmd.words);
	if (index < 0 || (size_t)index >= len)
		return (cmd);
	free(cmd.words[index]);
	cmd.words[index] = NULL;
	i = 0;
	while (i < len)
	{
		if (i + 1 < len && cmd.words[i] == NULL)
		{
			cmd.words[i] = cmd.words[i + 1];
			cmd.words[i + 1] = NULL;
		}
		i++;
	}
	return (cmd);
}

/* Waits the passed pit to exit, when it does
converts the exit status returned by waitpid(3)
into the corresponding exit code */
int	ft_wifexited(pid_t pid)
{
	int	status;

	waitpid(pid, &status, WUNTRACED);
	if ((status & 0xFF) == 0)
		return ((status >> 8) & 0xFF);
	else
		return (128 + (status & 0x7F));
	return (EXIT_SUCCESS);
}

static void	clean_exit(t_cmd *cmd_arr, char *line, char ***vars, int code)
{
	free_cmd(cmd_arr);
	mtx_free(vars[0]);
	mtx_free(vars[1]);
	free(line);
	exit(code);
}
