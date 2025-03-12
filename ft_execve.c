/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execve.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 18:32:32 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/11 15:30:52 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	clean_exit(char **arr, char *str, int *fd, int code)
{
	ft_freentf("12", str, arr);
	close(fd[0]);
	close(fd[1]);
	exit(code);
}

/* takes a in/out fd-array, the command string
and the env variables as parameters.
First checks if the command given is the path to an executable,
then checks if it's a command.
After that "links" (man 3 dup2) stdin = fd[0], stdout = fd[1].
then runs execve (man 3 execve) with the command
RETURNS: nothing :D, exits with EXIT_FAILURE status on error_check() error */
int	ft_execve(int *fd, const char *cmd, char **env)
{
	char	**args;
	char	*path;

	args = ft_split(cmd, ' ');
	if (!args)
		exit(EXIT_FAILURE);
	if (access(args[0], X_OK) == 0)
		path = args[0];
	else
		path = find_env_path(args[0], env);
	if (path == NULL)
	{
		ft_printfd(STDERR_FILENO, "minishell: command not found: %s\n", args[0]);
		clean_exit(args, path, fd, 127);
	}
	dup2(fd[0], STDIN_FILENO);
	dup2(fd[1], STDOUT_FILENO);
	execve(path, args, env);
	close(fd[0]);
	close(fd[1]);
	ft_freentf("2", args);
	free(path);
	exit(EXIT_FAILURE);
}
