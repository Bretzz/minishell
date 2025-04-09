/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execve.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totommi <totommi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 18:32:32 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/08 23:40:02 by totommi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "cmds.h"

int	is_directory(const char *path)
{
	DIR *dir;
	
	dir = opendir(path);
	if (dir == NULL)
		return (0);
	closedir(dir);
	return (1);
}

static void	clean_exit(char *str, int *fd)
{
	free(str);
	if (fd[0] > 2)
		close(fd[0]);
	if (fd[1] > 2)
		close(fd[1]);
	//exit(code);
}

/* returns the error code of the problem */
static int	error_check(t_cmd cmd, char *path)
{
	if (cmd.redir[1] == FILE
		&& access(cmd.outfile, F_OK) - access(cmd.outfile, W_OK) > 0)
	{
		ft_printfd(STDERR_FILENO, "minishell: %s: permission denied\n", cmd.outfile);
		return (1);
	}
	if (cmd.redir[0] == FILE
		&& access(cmd.infile, F_OK) != 0)
	{
		ft_printfd(STDERR_FILENO, "minishell: %s: no such file or directory\n", cmd.infile);
		return (1);
	}
	if (cmd.redir[0] == FILE
		&& access(cmd.infile, R_OK) != 0)
	{
		ft_printfd(STDERR_FILENO, "minishell: %s: permission denied\n", cmd.infile);
		return (1);
	}
	if (path == NULL)
	{
		ft_printfd(STDERR_FILENO, "minishell: %s: command not found\n", cmd.words[0]);
		return (127);
	}
	return (0);
}

/* takes a in/out fd-array, the command string
and the env variables as parameters.
First checks if the command given is the path to an executable,
then checks if it's a command.
After that "links" (man 3 dup2) stdin = fd[0], stdout = fd[1].
then runs execve (man 3 execve) with the command
RETURNS: 0 on bad pointer
		 127 on command not found
		 -1 on execve issue
NOTE: we are just closing the fds,
not freeing any external stuff (like the cmd array). */
int	ft_execve(int *fd, t_cmd cmd, char **env)
{
	char	*path;
	int		exit_status;

	path = NULL;
	if (!cmd.words[0])
		return (-1); // this should never happen (right now we just ignore, leaving all things open)
	if (is_directory(cmd.words[0]) == 1)
	{
		ft_printfd(STDERR_FILENO, "%s: Is a directory.\n", cmd.words[0]);
		return (126);
	}
	if (access(cmd.words[0], X_OK) == 0)
		path = ft_strdup(cmd.words[0]);
	else
		path = find_env_path(cmd.words[0], env);
	exit_status = error_check(cmd, path);
	if (exit_status != 0)
		return (clean_exit(path, fd), exit_status);
	if (DEBUG) {ft_printf("executing '%s' on: [%d,%d]\n", cmd.words[0], fd[0], fd[1]);}
	dup2(fd[0], STDIN_FILENO);
	dup2(fd[1], STDOUT_FILENO);
	safeclose(fd[0]);
	safeclose(fd[1]);
	// for (int i = 3; i < 260; i++)
	// {
	// if (fcntl(i, F_GETFD) != -1)
	// 	dprintf(2, ">> open fd: %d\n", i);
	// }
	execve(path, cmd.words, env);
//	ft_freentf("2", cmd.words);
	free(path);
	return (EXIT_FAILURE);
}
