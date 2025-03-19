/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 16:05:45 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/19 18:57:24 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int g_last_sig = 0;

static void	clean_exit(t_cmd *cmd_arr, char *line, char ***vars, int code)
{
	free_cmd(cmd_arr);
	ft_freentf("1222", line, vars[0], vars[1], vars[2]);
	exit(code);
}

int	ft_putchar(int c)
{
	return (write(STDOUT_FILENO, &c, 1));
}

static int	is_builtin(char *cmd)
{
	if (!ft_strncmp("echo", cmd, 5))
		return(1);
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
static int	exec_builtin(int *fd, t_cmd cmd, char ***vars)
{
	if (!ft_strncmp("echo", cmd.words[0], 5))
		return(ft_echo(fd, cmd, (const char ***)vars));
	else if (!ft_strncmp("cd", cmd.words[0], 3))
		return (ft_cd(fd, cmd));
	else if (!ft_strncmp("pwd", cmd.words[0], 4))
		return (ft_pwd(fd, cmd));
	else if (!ft_strncmp("export", cmd.words[0], 7))
		return (ft_export(fd, cmd, vars));
	else if (!ft_strncmp("unset", cmd.words[0], 6))
		return (ft_unset(fd, cmd, vars));
	else if (!ft_strncmp("env", cmd.words[0], 4))
		return (ft_env(fd, (const char ***)vars));
	else if (!ft_strncmp("exit", cmd.words[0], 5))
		return (multicose(fd), -1); //need to free command list
	return (0);
}

/* executes the command, setting g_pipe_status to the exit code of the command executed.
throughout the pipe, the main program keeps returning -1. (also execve if ragnarock occurs). */
static int	handle_command(t_cmd cmd, char ***vars)
{
	pid_t		pid;
	int			ret;
	int			fd[2];
	static int	oldfd[2]; // backup of the previous pipe

	if (handle_vars(cmd, vars))
		return (-1);
	if (is_builtin(cmd.words[0]))
	{
		miniwrapper(fd, oldfd, cmd);
		ret = exec_builtin(fd, cmd, vars);
		if (ret < 0)
		{
			//g_pipe_status = 0;
			vars[0] = setnum(vars[0], "LITTLEPIPE", 0);
			return (EXIT_SUCCESS);
		}
		//g_pipe_status = ret;
		vars[0] = setnum(vars[0], "LITTLEPIPE", ret);
		return (-1); // continue cycling trough commands
	}
	pid = wrapper(fd, oldfd, cmd);
	if (pid == 0)
	{
		ret = ft_execve(fd, cmd, vars[2]);
		return (ret);
	}
	waitpid(pid, &ret, WUNTRACED);
	//g_pipe_status = ((ret) & 0xff00) >> 8;
	vars[0] = setnum(vars[0], "LITTLEPIPE", ((ret) & 0xff00) >> 8);
	//ft_printf("status=%d\n", g_pipe_status);
	return (-1);
}

static int	handle_line(char *line, char ***vars)
{
	int	i;
	int	len;
	int	errno;
	t_cmd *cmd_arr;
	
	if (line == NULL)
		return (0);
	cmd_arr = parse_tokens((char *)line);
	len = ft_cmdlen(cmd_arr);
	//ft_printf("found %d command(s)\n", len);
	i = 0;
	while(i < len)
	{
		errno = handle_command(cmd_arr[i], vars);
		if (errno >= 0)
			clean_exit(cmd_arr, line, vars, errno);
		i++;
	}
	free_cmd(cmd_arr);
	//add_history(line);
	return (1);
}

static char	**env_copy(char **his_env)
{
	int	i;
	char **my_env;

	if (his_env == NULL)
		return (NULL);
	i = 0;
	my_env = (char **)ft_calloc((ft_mtxlen((const void **)his_env) + 1), sizeof(char *));
	if (my_env == NULL)
		return (NULL);
	while(his_env[i] != NULL)
	{
		my_env[i] = ft_strdup(his_env[i]);
		i++;
	}
	return (my_env);
}

/* LITTLEPIPE=errno.
later inplementation of:
PIPESTATUS
	An array variable de containing a list of
	exit status values from the processes in the most-recently-executed foreground
	pipeline (which may contain only a single command). */
static char	**pipe_setup(void)
{
	char	**mtx;

	mtx = var_append(NULL, "LITTLEPIPE=0");
	return (mtx);
}

int	main(int argc, char *argv[], char *__environ[])
{
	char	*line;
	char	**vars[3]; //vars[0]: shv, var[1]: exv, var[2]: env
	
	(void)argc; (void)argv;
	ft_bzero(vars, 3 * sizeof(char **));
	vars[2] = env_copy(__environ);
	vars[0] = pipe_setup();
	if (!vars[0] || !vars[2])
		return (1);
		
	ft_printf("\033[H\033[J"); // ANSI escape sequence to clear screen
	while (1)
	{
		line = ft_readline("minishell% ");
		if (!handle_line(line, vars))
			return (1);
	//	add_history(line);
		free(line);
	}
	return (0);
}
