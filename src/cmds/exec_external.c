/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_external.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 18:02:43 by march             #+#    #+#             */
/*   Updated: 2025/04/11 17:35:54 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "cmds.h"

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
		return (EXIT_FAILURE);
	}
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		exit_code = ft_execve(execfd, cmd[0], vars[1] + 1);
		clean_exit(cmd, line, vars, exit_code);
	}
	safeclose(execfd[0]);
	safeclose(execfd[1]);
	return (ft_wifexited(pid));
}
