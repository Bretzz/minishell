/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 18:57:25 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/11 18:16:51 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "cmds.h"

int		execute_command(char *line, t_cmd *cmd, char ***vars);

static void	close_all_fds(int *execfd)
{
	safeclose(execfd[0]);
	safeclose(execfd[1]);
}

/* execute the first command and returns the exit code.
returns the EXIT_STATUS of the command.
(CLEAN_EXIT if 'exit' command is executed).
NOTE: if the redir out for the single command is PIPE (as in 'ls | > out')
a dummy-pipe is created and the read-end is closed right ahead (SIGPIPE) */
int	execute_command(char *line, t_cmd *cmd, char ***vars)
{
	int		execfd[2];
	int		exit_code;

	execfd[0] = command_redir_input(cmd);
	execfd[1] = command_redir_output(cmd);
	if (ft_strichr(cmd->words[0], '=') != 0)
	{
		if (cmd->words[1] != NULL)
			cmd[0] = pop_arg(cmd[0], 0);
		else
		{
			handle_vars(cmd[0], vars);
			return (close_all_fds(execfd), EXIT_SUCCESS);
		}
	}
	if (is_builtin(cmd->words[0]))
	{
		exit_code = exec_builtin(execfd, cmd[0], vars);
		return (close_all_fds(execfd), exit_code);
	}
	exit_code = exec_external(execfd, line, cmd, vars);
	return (exit_code);
}
