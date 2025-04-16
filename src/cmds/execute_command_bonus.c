/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 11:16:27 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/16 15:58:57 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"
#include "cmds_bonus.h"

int	execute_command_bonus(char *line, t_bush *bush, t_cmd *cmd, char ***vars);

static void	clean_exit(t_cmd *cmd_arr, t_bush *bush, char ***vars, int code)
{
	free_cmd(cmd_arr);
	mtx_free(vars[0]);
	mtx_free(vars[1]);
	free(bush->line);
	free_bush(bush);
	exit(code);
}

/* Launches an ft_execve inside a fork for the single command.
Returns the exit code of ft_execve, which is either 127, 1,
the one of the command launched, or -1 (in really bad cases) */
static int	exec_external(int *execfd, t_bush *bush, t_cmd *cmd, char ***vars)
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
		clean_exit(cmd, bush, vars, exit_code);
	}
	safeclose(execfd[0]);
	safeclose(execfd[1]);
	return (ft_wifexited(pid));
}

/* execute the first command and returns the exit code.
returns the EXIT_STATUS of the command.
(CLEAN_EXIT if 'exit' command is executed).
NOTE: if the redir out for the single command is PIPE (as in 'ls | > out')
a dummy-pipe is created and the read-end is closed right ahead (SIGPIPE) */
int	execute_command_bonus(char *line, t_bush *bush, t_cmd *cmd, char ***vars)
{
	int		execfd[2];
	int		exit_code;

	(void)line;
	execfd[0] = command_redir_input(cmd);
	execfd[1] = command_redir_output(cmd);
	if (ft_strichr(cmd->words[0], '=') != 0)
	{
		if (cmd->words[1] != NULL)
			cmd[0] = pop_arg(cmd[0], 0);
		else if (handle_vars(cmd[0], vars))
			return (multicose(execfd), EXIT_SUCCESS);
	}
	if (is_builtin(cmd->words[0]))
	{
		exit_code = exec_builtin(execfd, cmd[0], vars);
		safeclose(cmd->close_me);
		if (!ft_strncmp("exit", cmd->words[0], 5))
			return (multicose(execfd), -1 * (exit_code + 1));
		return (multicose(execfd), exit_code);
	}
	exit_code = exec_external(execfd, bush, cmd, vars);
	return (exit_code);
}
