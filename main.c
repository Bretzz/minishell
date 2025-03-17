/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 16:05:45 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/17 21:09:04 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	clean_exit(t_cmd *cmd_arr, char *line, char **shv, char **env, int code)
{
	free_cmd(cmd_arr);
	ft_freentf("122", line, shv, env);
	exit(code);
}

int	ft_putchar(int c)
{
	return (write(STDOUT_FILENO, &c, 1));
}

/* returns 0 if command isn't builtin, 1 if it is
2 if it's echo -n. */
int		exec_builtin(t_cmd cmd, char ***shv, char ***env)
{
	if (!ft_strncmp("echo", cmd.words[0], 4))
		return(ft_echo(cmd, (const char**) *shv, (const char**) *env));
	else if (!ft_strncmp("cd", cmd.words[0], 2))
		return (ft_cd(cmd));
	else if (!ft_strncmp("pwd", cmd.words[0], 3))
		return (ft_pwd(cmd));
	else if (!ft_strncmp("export", cmd.words[0], 6))
		return (ft_export(cmd, shv, env));
	else if (!ft_strncmp("unset", cmd.words[0], 5))
		return (ft_unset(cmd, shv, env));
	else if (!ft_strncmp("env", cmd.words[0], 3))
		return (ft_env(*env));
	else if (!ft_strncmp("exit", cmd.words[0], 4))
		return (-1); //need to free command list
	return (0);
}

static int	handle_command(t_cmd cmd, char ***shv, char ***env)
{
	pid_t	pid;
	int		fd[2];
	int		ret;


	ret = exec_builtin(cmd, shv, env);
	if (ret != 0)
		return (ret);
	if (handle_vars(cmd, shv, env))
		return (1);
	pid = wrapper(&fd[0], cmd);
	if (pid == 0)
	{
		ret = ft_execve(fd, cmd, *env); //fix return command value (127 on command not found)
		if (ret == 127)
			return (-2);
		return (-1);
	}
	waitpid(pid, NULL, WUNTRACED);
	return (1);
}

static int	handle_line(char *line, char ***shv, char ***env)
{
	int	i;
	int	len;
	int	ret;
	t_cmd *cmd_arr;
	
	if (line == NULL)
		return (0);
	cmd_arr = parse_tokens((char *)line);
	len = ft_cmdlen(cmd_arr);
	i = 0;
	while(i < len)
	{
		ret = handle_command(cmd_arr[i], shv, env);
		if (ret == -1)
			clean_exit(cmd_arr, line, *shv, *env, EXIT_SUCCESS);
		if (ret == -2)
			clean_exit(cmd_arr, line, *shv, *env, 127);
		
		//free(cmd_arr[i].words);
		i ++;
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
	my_env = (char **)malloc((ft_mtxlen((const void **)his_env) + 1) * sizeof(char *));
	if (my_env == NULL)
		return (NULL);
	while(his_env[i] != NULL)
	{
		my_env[i] = ft_strdup(his_env[i]);
		i++;
	}
	my_env[i] = NULL;
	return (my_env);
}

int	main(void)
{
	char	*line;
	char	**shv;
	char	**env;
	
	shv = NULL;
	env = env_copy(__environ);
	if (env == NULL)
		return (1);
		
	ft_printf("\033[H\033[J"); // ANSI escape sequence to clear screen
	while (1)
	{
		line = ft_readline("minishell% ");
		if (!handle_line(line, &shv, &env))
			return (1);
	//	add_history(line);
		free(line);
	}
	return (0);
}
