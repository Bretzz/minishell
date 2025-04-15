/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_bridge.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 19:04:34 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/14 20:29:53 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		is_builtin(char *cmd);
int		exec_builtin(int *fd, t_cmd cmd, char ***vars);
void	builtin_and_die(int *fd, int index, t_cmd *cmd_arr, char ***vars);

static void	clean_exit(t_cmd *cmd_arr, char *line, char ***vars, int code)
{
	free_cmd(cmd_arr);
	mtx_free(vars[0]);
	mtx_free(vars[1]);
	free(line);
	exit(code);
}

/* compares the string passed with all builtins available.
NOTE: the string must point at the first letter of the name
and NULL-terminate after the builtin name.
RETURNS: 1 if it's a builtin, 0 if it isn't. */
int	is_builtin(char *cmd)
{
	if (!ft_strncmp("echo", cmd, 5))
		return (1);
	else if (!ft_strncmp("cd", cmd, 3))
		return (1);
	else if (!ft_strncmp("pwd", cmd, 4))
		return (1);
	else if (!ft_strncmp("export", cmd, 7))
		return (1);
	else if (!ft_strncmp("unset", cmd, 6))
		return (1);
	else if (!ft_strncmp("env", cmd, 4))
		return (1);
	else if (!ft_strncmp("exit", cmd, 5))
		return (1);
	return (0);
}

/* run after an is_builtin() call
returns the command 'exit code', -1 on dangerous errors. */
int	exec_builtin(int *fd, t_cmd cmd, char ***vars)
{
	if (!ft_strncmp("echo", cmd.words[0], 5))
		return (ft_echo(fd, cmd));
	else if (!ft_strncmp("cd", cmd.words[0], 3))
		return (ft_cd(fd, cmd, vars));
	else if (!ft_strncmp("pwd", cmd.words[0], 4))
		return (ft_pwd(fd, cmd));
	else if (!ft_strncmp("export", cmd.words[0], 7))
		return (ft_export(fd, cmd, vars));
	else if (!ft_strncmp("unset", cmd.words[0], 6))
		return (ft_unset(fd, cmd, vars));
	else if (!ft_strncmp("env", cmd.words[0], 4))
		return (ft_env(fd, (const char ***)vars));
	else if (!ft_strncmp("exit", cmd.words[0], 5))
		return (ft_exit(fd, cmd, vars));
	return (0);
}

/* run after an is_builtin() call
exec_builtin() call then exits with the corresponding 'exit_code'.
NOTE: REMEMBER TO FREE THE LINE OF READLINE BEFORE builtin_and_die CALL. */
void	builtin_and_die(int *fd, int index, t_cmd *cmd_arr, char ***vars)
{
	int	exit_code;

	if (dup2(fd[0], STDIN_FILENO) < 0)
		clean_exit(cmd_arr, NULL, vars, EXIT_FAILURE);
	safeclose(fd[0]);
	exit_code = exec_builtin(fd, cmd_arr[index], vars);
	clean_exit(cmd_arr, NULL, vars, exit_code);
}
