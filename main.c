/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 16:05:45 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/06 20:26:26 by topiana-         ###   ########.fr       */
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

/* executes the command, setting g_pipe_status to the exit code of the command executed.
throughout the pipe, the main program keeps returning -1. (also execve if ragnarock occurs). */
/* static int	handle_command(t_cmd cmd, char ***vars)
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
} */

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
	input_initializer();
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

static char *close_unclosed(char *line)
{
	char	*next_line;
	char	quote;
	char	last;
	size_t	i;

	if (line == NULL)	//never happens
		return (NULL);
	last = 0;
	quote = 0;
	i = 0;
	while (line[i] != '\0')
	{
		if (!ft_isspace(line[i]))
			last = line[i];
		if (quote == 0
			&& (line[i] == '\'' || line[i] == '"'))
			quote = line[i];
		else if (line[i] == quote)
			quote = 0;
		i++;
	}
	input_initializer();
	if (quote != 0)
	{
		next_line = readline("> ");
		line = close_unclosed(ft_strjoin_free_nl(line, next_line));
		free(next_line);
	}
	else if (last == '|'/*  || last == '&' */) //also for "||", "&" and "&&"
	{
		next_line = readline("> ");
		line = close_unclosed(ft_strjoin_free_space(line, next_line));
		free(next_line);
	}
	idle_initializer();
	return (line);
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
	{
		mtx_free(vars[0]);
		mtx_free(vars[1]);
		return (1);
	}

	//ft_printf("\033[H\033[J"); // ANSI escape sequence to clear screen
	rl_catch_signals = 0;	//MacOS issues... but what does it do?
	while (1)
	{
		idle_initializer();
		line = readline("minishell% ");
		//check g_last_signal
		// ft_signals (che chiama signal con i vari SIGNORE DEF o la tua function)
		if (line == NULL)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			clean_exit(NULL, NULL, vars, EXIT_SUCCESS);
		}
		line = close_unclosed(line);
		if (line == NULL)
			write(STDOUT_FILENO, "minishell: malloc failure\n", 26);
		else if (!handle_line(line, vars))
			return (1);
		else
			add_history(line);
		free(line);
	}
	return (0);
}
