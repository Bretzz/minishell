/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 16:05:45 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/09 18:43:32 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int g_last_sig = 0;

static void	clean_exit(t_cmd *cmd_arr, char *line, char ***vars, int code)
{
	free_cmd(cmd_arr);
	mtx_free(vars[0]);
	mtx_free(vars[1]);
	//mtx_free(vars[2]);
	free(line);
	exit(code);
}

// static void	clean_exit(t_cmd *cmd_arr, char *line, char ***vars, int code)
// {
// 	free_cmd(cmd_arr);
// 	//ft_freentf("1222", line, vars[0], vars[1], vars[2]);
// 	exit(code);
// }

int	ft_putchar(int c)
{
	return (write(STDOUT_FILENO, &c, 1));
}

static int	handle_line(char *line, char ***vars)
{
	int		exit_status;
	t_cmd 	*cmd_arr;
	size_t	len;
	
	cmd_arr = NULL;
	exit_status = 0;
	if (line == NULL)
		return (1);
	cmd_arr = parse_tokens((char *)line, (const char ***)vars);
	len = ft_cmdlen(cmd_arr);
	
	if (len == 0)	//both NULL cmd_arr and no actual command to execute
		return (free_cmd(cmd_arr), 1);
	runtime_initializer();
	if (len == 1)
	{
		if (DEBUG) {ft_printf("EXECUTING COMMAND\n");}
		exit_status = execute_command(line, cmd_arr, vars);
		if (exit_status < 0)
			clean_exit(cmd_arr, line, vars, EXIT_SUCCESS);
		mtx_setdata(exit_status, vars[0]);
	}
	else
	{
		if (DEBUG) {ft_printf("EXECUTING PIPELINE\n");}
		exit_status = execute_pipeline(line, cmd_arr, vars);
		if (exit_status < 0)
			clean_exit(cmd_arr, line, vars, EXIT_SUCCESS);
		mtx_setdata(exit_status, vars[0]);
	}
	free_cmd(cmd_arr);
	return (1);
}

static void	level_up(char ***env)
{
	char	value[12];
	int		new_val;

	mtx_findval("SHLVL", value, sizeof(value), *env);
	if (value[0] == '\0')
		new_val = 1;
	else
		new_val = ft_atoi(value) + 1;
	*env = mtx_setnum("SHLVL", new_val, *env);
}
/* copies the enviroment passed as parameters and returns
the newly initialized matrix. */
/* ! ! ! SETUP SHLVL, PWD, TMPDIR ! ! ! */
static char	**env_init(char **his_env)
{
	size_t	i;
	char **my_env;

	if (his_env == NULL)
		return (NULL);
	my_env = mtx_init();
	if (my_env == NULL)
		return (NULL);
	i = 0;
	while(his_env[i] != NULL)
	{
		my_env = mtx_vstr_copy(his_env[i], my_env);
		i++;
	}
	level_up(&my_env);
	return (my_env);
}

static char	*history_is_set(char *line)
{
	char	*strip_line;
	
	if (line == NULL)
		return (NULL);
	add_history(line);
	strip_line = drop_comment(line);
	free(line);
	return (strip_line);
}

int	main(int argc, char *argv[], char *__environ[])
{
	char	*line;
	char	**vars[2]; //vars[0]: shv, var[1]: env, var[2]: env
	//char	stack[1000000000];

	//line = malloc(167772160000000000);
	(void)argc; (void)argv; (void)__environ;/*  (void)stack; */
	ft_bzero(vars, 2 * sizeof(char **));
	//vars[2] = env_copy(__environ);
	vars[1] = env_init(__environ);
	vars[0] = mtx_init();
	if (!vars[0] || !vars[1])
	{
		mtx_free(vars[0]);
		mtx_free(vars[1]);
		return (EXIT_FAILURE);
	}
	//ft_printf("\033[H\033[J"); // ANSI escape sequence to clear screen
	// rl_catch_signals = 0;	//MacOS issues... but what does it do?
	while (1)
	{
		idle_initializer();
		line = readline("minishell% ");
		//check g_last_signal
		// ft_signals (che chiama signal con i vari SIGNORE (Jhonny?) DEF o la tua function)
		if (line == NULL)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			clean_exit(NULL, NULL, vars, EXIT_SUCCESS);
		}
		line = syntax_line(line);
		if (line == NULL)
			mtx_setdata(2, vars[0]);
		line = history_is_set(line);
		if (line != NULL && !handle_line(line, vars))
			return (EXIT_FAILURE);
		free(line);
		close_docs();
	}
	return (EXIT_SUCCESS);
}
