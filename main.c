/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 16:05:45 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/14 19:38:39 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_last_sig = 0;

static void	clean_exit(t_cmd *cmd_arr, char *line, char ***vars, int code)
{
	free_cmd(cmd_arr);
	mtx_free(vars[0]);
	mtx_free(vars[1]);
	free(line);
	exit(code);
}

/* execute the command array passed.
If the 'exit' command is run as single command an exit(3) call
is performed with the exit code of the last foreground pipeline. */
static void	executioner(char *line, t_cmd *cmd_arr, size_t len, char ***vars)
{
	int	exit_code;
	
	exit_code = 0;
	if (*((unsigned char *)vars[0] + 6) == 1)
		runtime_initializer();
	if (len <= 1)
	{
		exit_code = execute_command(line, cmd_arr, vars);
		if (exit_code < 0)
			clean_exit(cmd_arr, line, vars, -1 * (exit_code + 1));
		mtx_setdata(exit_code, vars[0], 1);
	}
	else
	{
		exit_code = execute_pipeline(line, cmd_arr, vars);
		if (exit_code < 0)
			clean_exit(cmd_arr, line, vars, -1 * (exit_code + 1));
		mtx_setdata(exit_code, vars[0], 1);
	}
	if (*((unsigned char *)vars[0] + 6) == 1)
		idle_initializer();
}

static int	king_richard_the_third(char *line, char ***vars)
{
	t_cmd	*cmd_arr;
	size_t	len;
	
	cmd_arr = NULL;
	if (line == NULL)
		return (1);
	cmd_arr = parse_tokens((char *)line, (const char ***)vars);
	if (g_last_sig != 0)
	{
		mtx_setdata(128 + g_last_sig, vars[0], 1);
		g_last_sig = 0;
		return (free_cmd(cmd_arr), 1);
	}
	len = ft_cmdlen(cmd_arr);
	if (len == 0)
	{
		mtx_setdata(cmd_arr->parse_code, vars[0], 1);
		return (free_cmd(cmd_arr), 1);
	}
	executioner(line, cmd_arr, len, vars);
	free_cmd(cmd_arr);
	return (1);
}

/* adds the passed line to the history, then alloc(3)s
a new string without comments (#), free(3)s the originl
string and returns the new string. */
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

/* if the SHLVL var is present increments it's value by 1. */
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
	while (his_env[i] != NULL)
	{
		my_env = mtx_vstr_copy(his_env[i], my_env);
		i++;
	}
	level_up(&my_env);
	return (my_env);
}

/* if a parameter is passed and it's a valid file, then
a dup2(3) call is performed so that STDIN_FILENO is pointing
to an fd open(3)ed on that file. */
static void stdin_init(int argc, char **argv, char ***vars)
{
	if (argc <= 1)
		return ;
	if (ft_strlen(argv[1]) > MAX_PATH)
	{
		ft_perror(argv[1], "File name too long", NULL, 126);
		clean_exit(NULL, NULL, vars, 126);
	}
	if (access(argv[1], F_OK) != 0)
	{
		ft_perror(argv[1], "No such file or directory", NULL, 127);
		clean_exit(NULL, NULL, vars, 127);
	}
	if (access(argv[1], R_OK) != 0)
	{
		ft_perror(argv[1], "Permission denied", NULL, 126);
		clean_exit(NULL, NULL, vars, 126);
	}
	int fd = open(argv[1], O_RDONLY);
	if (fd < 0 || dup2(fd, STDIN_FILENO) < 0)
	{
		safeclose(fd);
		ft_perror(argv[1], "Unknown error", NULL, 1);
		clean_exit(NULL, NULL, vars, EXIT_FAILURE);
	}
	close(fd);
}

/* sets the sigflag 'private space' of vars[0]
based on the attributes of the STDIN_FILENO. */
static void	sigflag_init(char ***vars)
{
	if (isatty(STDIN_FILENO))
	{
		mtx_setdata(1, vars[0], 2);
	}
	else
	{
		mtx_setdata(0, vars[0], 2);
	}
}

/* Initializes both vars[0] and vars[1], then setsup vars[1]
to be the env. if something went wrong an exit(3)s with
exit code EXIT_FAILURE */
static void	vars_init(char **__environ, char ***vars)
{
	ft_bzero(vars, 2 * sizeof(char **));
	vars[1] = env_init(__environ);
	vars[0] = mtx_init();
	if (!vars[0] || !vars[1])
	{
		mtx_free(vars[0]);
		mtx_free(vars[1]);
		exit(EXIT_FAILURE);
	}
}

static char	*get_my_line_pls(int sigflag)
{
	if (sigflag == 1)
	{
		idle_initializer();
		return (readline("minishell% "));
	}
	else
		return (trim_back_nl(get_next_line(STDIN_FILENO)));
}

static void	handle_line(char **line, char ***vars)
{
	int exit_code;
	
	if (*line == NULL)
	{
		if (*((unsigned char *)vars[0] + 6) == 1)
			write(STDOUT_FILENO, "exit\n", 5);
		clean_exit(NULL, NULL, vars, *((unsigned char *)vars[0] + 5));
	}
	exit_code = syntax_line(line, vars);
	if (exit_code == 2)
		bongou_stray_docs(*line, (const char ***)vars);
	else if (exit_code == 1)
		clean_exit(NULL, *line, vars, EXIT_FAILURE);
	*line = history_is_set(*line);
	if (*line != NULL && exit_code == 0 && !king_richard_the_third(*line, vars))
		clean_exit(NULL, *line, vars, EXIT_FAILURE);
}

int	main(int argc, char *argv[], char *__environ[])
{
	char	*line;
	char	**vars[2];
//	int		exit_code;
	
	vars_init(__environ, vars);
	stdin_init(argc, argv, vars);
	sigflag_init(vars);
	while (1)
	{
		line = get_my_line_pls(*((unsigned char *)vars[0] + 6));
		handle_line(&line, vars);
		// if (line == NULL)
		// {
		// 	if (*((unsigned char *)vars[0] + 6) == 1)
		// 		write(STDOUT_FILENO, "exit\n", 5);
		// 	clean_exit(NULL, NULL, vars, *((unsigned char *)vars[0] + 5));
		// }
		// exit_code = syntax_line(&line, vars);
		// if (exit_code == 2)
		// 	bongou_stray_docs(line, (const char ***)vars);
		// else if (exit_code == 1)
		// 	clean_exit(NULL, line, vars, EXIT_FAILURE);
		// line = history_is_set(line);
		// if (line != NULL && exit_code == 0 && !king_richard_the_third(line, vars))
		// 	clean_exit(NULL, line, vars, EXIT_FAILURE);
		free(line);
		close_docs();
	}
	return (*((unsigned char *)vars[0] + 5));
}

