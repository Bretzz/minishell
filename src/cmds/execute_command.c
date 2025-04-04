/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 18:57:25 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/04 22:59:50 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "cmds.h"

int		execute_command(char *line, t_cmd *cmd, char ***vars);
t_cmd	pop_arg(t_cmd cmd, int index);

static void	clean_exit(t_cmd *cmd_arr, char *line, char ***vars, int code)
{
	free_cmd(cmd_arr);
	mtx_free(vars[0]);
	mtx_free(vars[1]);
	free(line);
	exit(code);
}

t_cmd	pop_arg(t_cmd cmd, int index)
{
	size_t			len;
	unsigned int	i;

	len = ft_mtxlen((const void **)cmd.words);
	if (index < 0 || (size_t)index >= len)
		return (cmd);
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

/* Launches an ft_execve inside a fork for the single command.
Returns the exit code of ft_execve, which is either 127, 1, the one of the command launched
of -1 (in really bad cases) */
static int	fork_external(int *execfd, char *line, t_cmd *cmd, char ***vars)
{
	pid_t 	pid;
	int		exit_status;
	
	pid = fork();
	if (pid < 0)
	{
		write(STDERR_FILENO, "fork failure\n", 13);
		safeclose(execfd[0]);
		safeclose(execfd[1]);
		return (EXIT_FAILURE);
	}
	if (pid == 0)
	{
		exit_status = ft_execve(execfd, cmd[0], vars[1] + 1);
		clean_exit(cmd, line, vars, exit_status);
	}
	safeclose(execfd[0]);
	safeclose(execfd[1]);
	waitpid(pid, &exit_status, WUNTRACED);
	return (((exit_status) & 0xff00) >> 8);
}

/* execute the first command and returns the exit code.
returns the EXIT_STATUS of the command.
(CLEAN_EXIT if 'exit' command is executed) */
int	execute_command(char *line, t_cmd *cmd, char ***vars)
{
	int		execfd[2];

	execfd[0] = STDIN_FILENO;
	execfd[1] = STDOUT_FILENO;
	if (cmd[0].redir[0] == FILE || cmd[0].redir[0] == HERE_DOC)
		execfd[0] = cmd[0].fd[0];
	if (cmd[0].redir[1] == FILE)
		execfd[1] = cmd[0].fd[1];
	if (ft_strichr(cmd[0].words[0], '=') != 0)
	{
		if (cmd[0].words[1] != NULL)
			cmd[0] = pop_arg(cmd[0], 0);
		else
		{
			handle_vars(cmd[0], vars);
			safeclose(execfd[0]);
			safeclose(execfd[1]);
			return (EXIT_SUCCESS);
		}
	}
	if (is_builtin(cmd[0].words[0]))
		return (exec_builtin(execfd, cmd[0], vars));
	return (fork_external(execfd, line, cmd, vars));
}
