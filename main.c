/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 16:05:45 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/09 23:51:40 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_putchar(int c)
{
	return (write(STDOUT_FILENO, &c, 1));
}

/* returns 0 if commant isn't builtin, 1 if it is
2 if it's ecoh -n. */
int		exec_builtin(char *cmd)
{
	if (!ft_strncmp("echo", cmd, 4))
		return(ft_echo(cmd));
	else if (!ft_strncmp("cd", cmd, 2))
		return (ft_cd(cmd));
	else if (!ft_strncmp("pwd", cmd, 3))
		return (ft_pwd(cmd));
	else if (!ft_strncmp("export", cmd, 6))
		return (ft_printf("minishell: builtin in progress: %s\n", cmd));
	else if (!ft_strncmp("unset", cmd, 5))
		return (ft_printf("minishell: builtin in progress: %s\n", cmd));
	else if (!ft_strncmp("env", cmd, 3))
		return (ft_printf("minishell: builtin in progress: %s\n", cmd));
	else if (!ft_strncmp("exit", cmd, 4))
		return (free(cmd), exit(EXIT_SUCCESS), 1);
	return (0);
}

void	handle_command(char *cmd, char **env)
{
	pid_t	pid;
	int		fd[2];
	
	fd[0] = 0;
	fd[1] = 1;
	if (!exec_builtin(cmd))
	{
		pid = fork();
		if (pid == 0)
			ft_execve(fd, cmd, env);
		waitpid(pid, NULL, WUNTRACED);
	}
}

/*
var1=123
var2=laskd
var3=
var4=123;12k3
*/
char	*get_value(const char *vars, const char *target)
{
	size_t	tar_len;
	size_t	var_len;
	char	*my_tar;
	
	if (vars == NULL || target == NULL)
		return (NULL);
	my_tar = ft_strdup(target);
	tar_len = ft_strlen(my_tar);
	my_tar[tar_len] = '=';
	while (*vars != '\0')
	{
		if (!ft_strncmp(vars, my_tar, tar_len + 1))
		{
			free(my_tar);
			var_len = ft_strlen_nl(vars + tar_len + 1);
			return (ft_substr(vars, tar_len + 1, var_len));
		}
		while (*vars != '\0')
		{
			if (*(vars++) == '\n')
				break ;
		}
	}
	free(my_tar);
	return (NULL);
}

/* adds the var to the var_list or reads the var.
TODO:
	1. replacement of old value in exists.
	2. expand_string function. */
void	handle_vars(char *cmd, char **shell_var, char **__environ)
{
	char	*eq;

	(void)__environ;
	//ft_printf("cmd=%s\n", cmd);
	eq = ft_strchr(cmd, '=');
	//ft_printf("eq=%s\n", eq);
	if (eq != NULL && eq != cmd)
		*shell_var = ft_strjoin_free_space_nl(*shell_var, cmd);
	else if (!ft_strncmp("$", cmd, 1))
	{
		ft_printf("%s\n", get_value(*shell_var, ft_strchr(cmd, '$') + 1));
	}
	return ;
}

int	main(void)
{
	char	*cmd;
	char	*shell_var;
	
	shell_var = NULL;
	ft_printf("\033[H\033[J"); // ANSI escape sequence to clear screen
	while (1)
	{
		cmd = readline("minishell% ");
		if (cmd == NULL)
			return (1);
		handle_vars(cmd, &shell_var, __environ);
	//	handle_command(cmd, __environ);
		add_history(cmd);
		free(cmd);
	}
	return (0);
}
