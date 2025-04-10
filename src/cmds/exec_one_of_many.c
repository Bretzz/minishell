/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_one_of_many.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totommi <totommi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 15:07:49 by totommi           #+#    #+#             */
/*   Updated: 2025/04/10 15:35:49 by totommi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "cmds.h"

void	single_execute(int *execfd, int index, t_cmd *cmd, char ***vars);

static void	clean_exit(int *execfd, t_cmd *cmd_arr, char ***vars, int code)
{
	multicose(execfd);
	free_cmd(cmd_arr);
	mtx_free(vars[0]);
	mtx_free(vars[1]);
	exit(code);
}

/* Executes a single command */
void	single_execute(int *execfd, int index, t_cmd *cmd, char ***vars)
{
	int	exit_status;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (ft_strichr(cmd[index].words[0], '=') != 0)
	{
		if (cmd[index].words[1] != NULL)
			cmd[index] = pop_arg(cmd[index], 0);
		else
		{
			handle_vars(cmd[index], vars);
			clean_exit(execfd, cmd, vars, EXIT_SUCCESS);
		}
	}
	if (!is_builtin(cmd[index].words[0]))
	{
		exit_status = ft_execve(execfd, cmd[index], vars[1] + 1);
		clean_exit(execfd, cmd, vars, exit_status);
	}
	builtin_and_die(execfd, index, cmd, vars);
}
