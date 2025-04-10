/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_cute.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totommi <totommi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 13:46:21 by totommi           #+#    #+#             */
/*   Updated: 2025/04/10 16:04:36 by totommi          ###   ########.fr       */
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

/* Launches an ft_execve inside a fork for the single command.
Returns the exit code of ft_execve, which is either 127, 1,
the one of the command launched, or -1 (in really bad cases) */
int	exec_external(int *execfd, char *line, t_cmd *cmd, char ***vars)
{
	pid_t	pid;
	int		exit_code;

	pid = fork();
	if (pid < 0)
	{
		write(STDERR_FILENO, "minishell: fork failure\n", 24);
		safeclose(execfd[0]);
		safeclose(execfd[1]);
		safeclose(cmd->close_me);
		return (EXIT_FAILURE);
	}
	if (pid == 0)
	{
		safeclose(cmd->close_me);
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		exit_code = ft_execve(execfd, cmd[0], vars[1] + 1);
		clean_exit(cmd, line, vars, exit_code);
	}
	safeclose(execfd[0]);
	safeclose(execfd[1]);
	safeclose(cmd->close_me);
	return (ft_wifexited(pid));
}
