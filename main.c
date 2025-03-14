/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 16:05:45 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/13 13:04:05 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_putchar(int c)
{
	return (write(STDOUT_FILENO, &c, 1));
}

/* returns 0 if command isn't builtin, 1 if it is
2 if it's echo -n. */
int		exec_builtin(char *who, t_cmd cmd, char ***shv, char ***env)
{
	if (!ft_strncmp("echo", who, 4))
		return(ft_echo(cmd, *shv, *env));
	else if (!ft_strncmp("cd", who, 2))
		return (ft_cd(cmd));
	else if (!ft_strncmp("pwd", who, 3))
		return (ft_pwd(cmd));
	else if (!ft_strncmp("export", who, 6))
		return (ft_export(cmd, shv, env));
	else if (!ft_strncmp("unset", who, 5))
		return (ft_unset(cmd, shv, env));
	else if (!ft_strncmp("env", who, 3))
		return (ft_env(*env));
	else if (!ft_strncmp("exit", who, 4))
		return (ft_freentf("22", *shv, *env), exit(EXIT_SUCCESS), 1); //need to free command list
	return (0);
}

void	handle_command(t_cmd cmd, char ***shv, char ***env)
{
	pid_t	pid;
	int		fd[2];
	
	fd[0] = 0;
	fd[1] = 1;
	if (exec_builtin(cmd.words->value, cmd, shv, env))
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

int	main(void)
{
	char	*cmd;
	char	**shv;
	char	**env;
	
	shv = NULL;
	env = env_copy(__environ);
	if (env == NULL)
		return (1);
	ft_printf("\033[H\033[J"); // ANSI escape sequence to clear screen
	while (1)
	{
		cmd = ft_readline("minishell% ");
		if (cmd == NULL)
			return (1);
		handle_command(cmd, &shv, &env);
	//	add_history(cmd);
		free(cmd);
	}
	return (0);
}
