/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 16:05:45 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/02 22:05:31 by topiana-         ###   ########.fr       */
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
		return(ft_echo(fd, cmd));
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
		return (multicose(fd), -1); //need to free command list
	return (0);
}

/* executes the command, setting g_pipe_status to the exit code of the command executed.
throughout the pipe, the main program keeps returning -1. (also execve if ragnarock occurs). */
static int	handle_command(t_cmd cmd, char ***vars)
{
	pid_t			pid;
	int				ret;
	int				fd[2];
	static int		oldfd[2];
	struct sigaction	ignore_sig;

	sig_initializer();

	if (handle_vars(cmd, vars))
		return (-1);
	if (is_builtin(cmd.words[0]))
	{
		miniwrapper(fd, oldfd, cmd);
		ret = exec_builtin(fd, cmd, vars);
		if (ret < 0)
		{
			mtx_setdata(0, vars[0]);
			return (EXIT_SUCCESS);
		}
		mtx_setdata(ret, vars[0]);
		return (-1);
	}
	// Imposta SIGINT per essere ignorato nel processo padre
	sigemptyset(&ignore_sig.sa_mask);
	ignore_sig.sa_flags = 0;
	ignore_sig.sa_handler = SIG_IGN;
	sigaction(SIGINT, &ignore_sig, NULL);

	pid = wrapper(fd, oldfd, cmd);
	if (pid == 0)
	{
		// Nel figlio ripristina il comportamento di default per SIGINT e SIGQUIT
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		ret = ft_execve(fd, cmd, vars[1] + 1);
		return (ret);
	}
	waitpid(pid, &ret, WUNTRACED);
	// Se il figlio è terminato a causa di SIGINT, manda un newline
	if (WIFSIGNALED(ret) && WTERMSIG(ret) == SIGINT)
		write(STDOUT_FILENO, "\n", 1);
	// Riattiva il nostro handler personalizzato
	sig_initializer();
	mtx_setdata(((ret) & 0xff00) >> 8, vars[0]);
	return (-1);
}

static int	handle_line(char *line, char ***vars)
{
	int	errno;
	t_cmd *cmd_arr;
	size_t	i;
	size_t	len;
	
	if (line == NULL)
		return (0);
	cmd_arr = parse_tokens((char *)line, (const char ***)vars);
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
	return (1);
}

/* copies the enviroment passed as parameters and returns
the newly initialized matrix. */
static char	**env_copy(char **his_env)
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
	return (my_env);
}

int	main(int argc, char *argv[], char *__environ[])
{
	char	*line;
	char	**vars[2]; //vars[0]: shv, var[1]: env, var[2]: env
	//char	stack[1000000000];

	//line = malloc(167772160000000000);
	(void)argc; (void)argv; (void)__environ;/*  (void)stack; */
	ft_bzero(vars, 3 * sizeof(char **));
	//vars[2] = env_copy(__environ);
	vars[1] = env_copy(__environ);
	vars[0] = mtx_init();
	if (!vars[0] || !vars[1])
		return (1);

	sig_initializer();
	ft_printf("\033[H\033[J"); // ANSI escape sequence to clear screen
	rl_catch_signals = 0;
	while (1)
	{
		line = readline("minishell% ");
		//check g_last_signal
		// ft_signals (che chiama signal con i vari SIGNORE DEF o la tua function)
		if (line == NULL)
		{
			ft_printf("exit\n");
			clean_exit(NULL, NULL, vars, EXIT_SUCCESS);
		}
		if (!handle_line(line, vars))
			return (1);
		add_history(line);
		free(line);
	}
	return (0);
}
