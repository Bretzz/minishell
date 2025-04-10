/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totommi <totommi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 18:57:25 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/10 15:41:30 by totommi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "cmds.h"

int		execute_command(char *line, t_cmd *cmd, char ***vars);

static void	close_all_fds(int *execfd, int close_me)
{
	safeclose(execfd[0]);
	safeclose(execfd[1]);
	safeclose(close_me);
}

/* execute the first command and returns the exit code.
returns the EXIT_STATUS of the command.
(CLEAN_EXIT if 'exit' command is executed).
NOTE: if the redir out for the single command is PIPE (as in 'ls | > out')
a dummy-pipe is created and the read-end stored in cmd->close_me,
so that can be closed after the command execution. */
int	execute_command(char *line, t_cmd *cmd, char ***vars)
{
	int		execfd[2];
	int		exit_code;

	execfd[0] = command_redir_input(cmd);
	execfd[1] = command_redir_output(cmd);
	if (execfd[0] < 0 || execfd[1] < 0)
		return (close_all_fds(execfd, cmd->close_me), EXIT_FAILURE);
	if (ft_strichr(cmd->words[0], '=') != 0)
	{
		if (cmd->words[1] != NULL)
			cmd[0] = pop_arg(cmd[0], 0);
		else
		{
			handle_vars(cmd[0], vars);
			return (close_all_fds(execfd, cmd->close_me), EXIT_SUCCESS);
		}
	}
	if (is_builtin(cmd->words[0]))
	{
		exit_code = exec_builtin(execfd, cmd[0], vars);
		return (close_all_fds(execfd, cmd->close_me), exit_code);
	}
	exit_code = exec_external(execfd, line, cmd, vars);
	return (exit_code);
}
