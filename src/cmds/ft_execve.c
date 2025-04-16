/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execve.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 18:32:32 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/16 16:21:15 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "cmds.h"

static void	clean_exit(char *str, int *fd)
{
	free(str);
	if (fd[0] > 2)
		close(fd[0]);
	if (fd[1] > 2)
		close(fd[1]);
}

void	ft_perror(char *cmd, char *strerr, int *exit_code, int code)
{
	if (!strncmp("Is a directory", strerr, 15)
		|| !strncmp("command not found", strerr, 18))
		ft_printfd(STDERR_FILENO, "%s: %s\n", cmd, strerr);
	else
		ft_printfd(STDERR_FILENO, "minishell: %s: %s\n", cmd, strerr);
	if (exit_code != NULL)
		*exit_code = code;
}

/* checks if some redirections went wrong */
static int	redir_error_check(t_cmd cmd)
{
	if (cmd.redir[1] == FILE && cmd.fd[1] < 0)
		return (1);
	if (cmd.redir[0] == FILE && cmd.fd[0] < 0)
		return (1);
	return (0);
}

static char	*set_exec_path(t_cmd cmd, char **env, int *exit_code)
{
	char	*path;

	if (!cmd.words[0])
	{
		*exit_code = EXIT_FAILURE;
		return (NULL);
	}
	if (ft_strchr(cmd.words[0], '/'))
	{
		if (is_directory(cmd.words[0]) == 1)
			return (ft_perror(cmd.words[0],
					"Is a directory", exit_code, 126), NULL);
		if (access(cmd.words[0], F_OK) != 0 && errno == ENOENT)
			return (ft_perror(cmd.words[0],
					"No such file or directory", exit_code, 127), NULL);
		if (access(cmd.words[0], X_OK) != 0)
			return (ft_perror(cmd.words[0],
					"Permission denied", exit_code, 126), NULL);
		return (ft_strdup(cmd.words[0]));
	}
	path = find_env_path(cmd.words[0], env);
	if (path == NULL)
		return (ft_perror(cmd.words[0],
				"command not found", exit_code, 127), NULL);
	return (path);
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
	int		exit_code;

	path = NULL;
	exit_code = 0;
	exit_code = redir_error_check(cmd);
	if (exit_code != 0)
		return (clean_exit(path, fd), exit_code);
	path = set_exec_path(cmd, env, &exit_code);
	if (exit_code != 0)
		return (clean_exit(path, fd), exit_code);
	dup2(fd[0], STDIN_FILENO);
	dup2(fd[1], STDOUT_FILENO);
	safeclose(fd[0]);
	safeclose(fd[1]);
	execve(path, cmd.words, env);
	free(path);
	return (EXIT_FAILURE);
}
