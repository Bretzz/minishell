/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapascal <mapascal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 16:05:45 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/17 18:35:16 by mapascal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		return (ft_freentf("22", *shv, *env), exit(EXIT_SUCCESS), 1); //need to free command list
	return (0);
}

void	handle_command(t_cmd cmd, char ***shv, char ***env)
{
	pid_t	pid;
	int		fd[2];

	fd[0] = 0;
	fd[1] = 1;
	if (exec_builtin(cmd, shv, env))
		return ;
	if (handle_vars(cmd, shv, env))
		return ;
	pid = fork();
	if (pid == 0)
		ft_execve(fd, cmd, *env);
	waitpid(pid, NULL, WUNTRACED);
}

char	**env_copy(char **his_env)
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

// static void	free_cmd(t_cmd *cmd_arr)
// {
// 	int	i;

// 	if (cmd_arr == NULL)
// 		return ;
// 	i = 0;
// 	while (cmd_arr[i].words[0][0] != '\0')
// 	{
// 		ft_freentf("2", cmd_arr[i].words);
// 		i++;
// 	}
// 	free(cmd_arr);
// }

static int	handle_line(const char *line, char ***shv, char ***env)
{
	int	i;
	int	len;
	t_cmd *cmd_array;

	if (line == NULL)
		return (0);
	cmd_array = parse_tokens((char *)line);
	len = ft_cmdlen(cmd_array);
	i = 0;
	while(i < len)
	{
		handle_command(cmd_array[i], shv, env);
		//free(cmd_array[i].words);
		i ++;
	}
	free/* _cmd */(cmd_array);
	//add_history(line);
	return (1);
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
