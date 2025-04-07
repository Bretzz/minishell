/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 18:57:25 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/07 18:31:15 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "cmds.h"

int		execute_command(char *line, t_cmd *cmd, char ***vars);
t_cmd	pop_arg(t_cmd cmd, int index);
int		dummy_in_pipe(void);

static void	clean_exit(t_cmd *cmd_arr, char *line, char ***vars, int code)
{
	free_cmd(cmd_arr);
	mtx_free(vars[0]);
	mtx_free(vars[1]);
	free(line);
	exit(code);
}

int	dummy_in_pipe(void)
{
	int	pipefd[2];
	
	pipefd[0] = STDIN_FILENO;
	pipefd[1] = STDOUT_FILENO;
	if (pipe(pipefd) < 0)
	{
		write(STDERR_FILENO, "minishell: pipe failure\n", 24);
		return (STDIN_FILENO);
	}
	safeclose(pipefd[1]);
	return (pipefd[0]);
}

t_cmd	pop_arg(t_cmd cmd, int index)
{
	size_t			len;
	unsigned int	i;

	len = ft_mtxlen((const void **)cmd.words);
	if (index < 0 || (size_t)index >= len)
		return (cmd);
	free(cmd.words[index]);
	cmd.words[index] = NULL;
	i = 0;
	while (i < len)
	{
		if (i + 1 < len && cmd.words[i] == NULL)
		{
			cmd.words[i] = cmd.words[i + 1];
			cmd.words[i + 1] = NULL;
		}
		i++;
	}
	return (cmd);
}

/* Launches an ft_execve inside a fork for the single command.
Returns the exit code of ft_execve, which is either 127, 1, the one of the command launched
of -1 (in really bad cases) */
static int	fork_external(int *execfd, char *line, t_cmd *cmd, char ***vars)
{
	pid_t 	pid;
	int		exit_status;
	
	pid = fork();
	if (pid < 0)
	{
		write(STDERR_FILENO, "minishell: fork failure\n", 24);
		safeclose(execfd[0]);
		safeclose(execfd[1]);
		safeclose(cmd->close_me);
		return (EXIT_FAILURE);
	}
	if (pid == 0)
	{
		safeclose(cmd->close_me);
		/* Nel figlio ripristina il comportamento di default per SIGINT e SIGQUIT */
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		exit_status = ft_execve(execfd, cmd[0], vars[1] + 1);
		clean_exit(cmd, line, vars, exit_status);
	}
	safeclose(execfd[0]);
	safeclose(execfd[1]);
	waitpid(pid, &exit_status, WUNTRACED);
	safeclose(cmd->close_me);
	return (((exit_status) & 0xff00) >> 8);
}

/* execute the first command and returns the exit code.
returns the EXIT_STATUS of the command.
(CLEAN_EXIT if 'exit' command is executed).
NOTE: if the redir for the single command is PIPE (as in 'ls | > out')
a dummy-pipe is created and stored in cmd->fd, so that can be
closed after the command execution. */
int	execute_command(char *line, t_cmd *cmd, char ***vars)
{
	int		execfd[2];
	int		pipefd[2];
	int		exit_status;

	execfd[0] = STDIN_FILENO;
	execfd[1] = STDOUT_FILENO;
	if (cmd->redir[0] == FILE || cmd->redir[0] == HERE_DOC)
		execfd[0] = cmd->fd[0];
	if (cmd->redir[1] == FILE)
		execfd[1] = cmd->fd[1];
	if (cmd->redir[1] == PIPE)
	{
		pipefd[0] = STDIN_FILENO;
		pipefd[1] = STDOUT_FILENO;
		if (pipe(pipefd) < 0)	//dummy pipe creation
		{
			write(STDERR_FILENO, "minishell: pipe failure\n", 25);
			// ...or just return
		}
		else
		{
			execfd[1] = pipefd[1];
			cmd->close_me = pipefd[0];
		}
		//safeclose(pipefd[0]);
	}
	if (cmd->redir[0] == PIPE)
		execfd[0] = dummy_in_pipe();
	if (ft_strichr(cmd->words[0], '=') != 0)
	{
		if (cmd->words[1] != NULL)
			cmd[0] = pop_arg(cmd[0], 0);
		else
		{
			handle_vars(cmd[0], vars);
			close_docs();
			safeclose(execfd[0]);
			safeclose(execfd[1]);
			safeclose(cmd->close_me);
			return (EXIT_SUCCESS);
		}
	}
	if (is_builtin(cmd->words[0]))
	{
		exit_status = exec_builtin(execfd, cmd[0], vars);
		close_docs();
		safeclose(execfd[0]);
		safeclose(cmd->close_me);
		return (exit_status);
	}
	exit_status = fork_external(execfd, line, cmd, vars);
	close_docs();
	return (exit_status);
}
