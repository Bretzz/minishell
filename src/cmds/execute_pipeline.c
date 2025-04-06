/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 11:58:38 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/06 18:59:49 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "cmds.h"

#define INPUT 0 /* Reassign of default INPUT value to closed fd. */
#define OUTPUT 1 /* Reassign of default OUTPUT value to closed fd. */

int	execute_pipeline(char *line, t_cmd *cmd_arr, char ***vars);

static void	clean_exit(int *execfd, t_cmd *cmd_arr, char ***vars, int code)
{
	multicose(execfd);
	free_cmd(cmd_arr);
	mtx_free(vars[0]);
	mtx_free(vars[1]);
	//mtx_free(vars[2]);
	exit(code);
}

/* This part of the code expects an array (2+) of cmds all separated by pipes.
if there are multiple commands it links them in pipes.
NOTE: All variables are already expanded, all heredocs filled and files (read/write) opened. */

static void	cleanclose(int *fd, char flag)
{
	if (*fd > 2)
		close(*fd);
	if (flag == INPUT)
		*fd = 0;
	if (flag == OUTPUT)
		*fd = 1;
	else
		*fd = -1;
}

/* returns the fd where to run execve.
If the cmd at 'index' already has a preferred input closes the previous
read-end of the pipe, then returns the fd, otherwise returns the read-end
of the previous pipe.
If we are at the first command and no preferred fd is specified, returns STDIN_FILENO.
NOTE: with 'preferred fd' we are refering to tho the fd written in the t_cmd struct */
static int	redir_input(t_cmd *cmd, t_garb *garb, int index, size_t len)
{
	if (DEBUG) {ft_printf("redir[0]: %d\n", cmd[index].redir[0]);}
	(void)len;
	if (cmd[index].redir[0] == FILE || cmd[index].redir[0] == HERE_DOC)
		return(cmd[index].fd[0]);
	if (index == 0)
	{
		if (cmd[index].redir[0] == PIPE)
			return (dummy_in_pipe());
		return (STDIN_FILENO);
	}
	if (index > 0 && cmd[index - 1].redir[1] == FILE)
		return (STDIN_FILENO);
	return (garb[index - 1].pipefd[0]);
}

/* If the cmd at 'index' has a preferred ouput just returns it, otherwise creates a new
pipe and returns the write-end of that pipe.
If we are at the last command and no preferred fd is specified, returns STDOUT_FILENO.
NOTE: with 'preferred fd' we are refering to tho the fd written in the t_cmd struct */
static int	redir_output(t_cmd *cmd, t_garb *garb, int index, size_t len)
{
	if (cmd[index].redir[1] == FILE)
	{
		return(cmd[index].fd[1]);
	}
	if (index == (int)(len - 1))
		return (STDOUT_FILENO);
	if (pipe(garb[index].pipefd) < 0)
	{
		garb[index].pipefd[0] = STDIN_FILENO;
		garb[index].pipefd[1] = STDOUT_FILENO;
		write(STDERR_FILENO, "minishell: pipe failure\n", 24);
		return (STDOUT_FILENO);
	}
	return (garb[index].pipefd[1]);
}

/* Closes all the fds of the files opened for the next command(s) in line.
Then closes all the fds of the previous 'hanging pipes', ex: cat | <file grep "str". */
static void carpet_close(t_cmd *cmd, t_garb *garb, int index, size_t len)
{
	unsigned int	i;

	(void)garb;
	i = index + 1;
//	ft_printf("carpet closing of len: %d, starting: %d\n", len, i);
	while (i < len)
	{
		if (cmd[i].redir[0] == FILE || cmd[i].redir[0] == HERE_DOC)
			safeclose(cmd[i].fd[0]);
		if (cmd[i].redir[1] == FILE)
			safeclose(cmd[i].fd[1]);
		i++;
	}
	if (index == 0)
		return ;
	i = 0;
	while (i < (unsigned int)index - 1)
	{
		safeclose(garb[i].pipefd[0]);
		safeclose(garb[i].pipefd[1]);
		i++;
	}
}

/* To be called in the father after a fork.
Closing pipes and fds of current command that aren't used in following commands.
Namely: the current write-end pipefd (used by the child we just forked),
The input file (FILE or HERE_DOC) and the output file (if existing). */
static void	father_closes(t_cmd *cmd, t_garb *garb, int index, size_t len)
{
	if (index != (int)(len - 1) && cmd[index].redir[1] != FILE)
	{
		cleanclose(&garb[index].pipefd[1], OUTPUT);
	}
	if (cmd[index].redir[0] == FILE || cmd[index].redir[0] == HERE_DOC)
	{
		safeclose(cmd[index].fd[0]);
	}
	if (cmd[index].redir[1] == FILE)
	{
		safeclose(cmd[index].fd[1]);
	}
}

/* To be calld in the child after a fork.
Closes all the fds that aren't useful to the execution of the current command.
Namely: the read-end of the current pipe,
the write-end of the previous pipe (if exists),
the read-end of the previous pipe (if we read from a FILE/HERE_DOC).
All of this after a carpet_close() call. */
static void	child_closes(t_cmd *cmd, t_garb *garb, int index, size_t len)
{
	carpet_close(cmd, garb, index, len);
	if (index != (int)(len - 1) && cmd[index].redir[1] != FILE)
	{
		safeclose(garb[index].pipefd[0]);
	}
	if (index > 0 && cmd[index - 1].redir[1] != FILE)
	{
		safeclose(garb[index - 1].pipefd[1]);
	}
	if (index > 0 && cmd[index].redir[0] == FILE)
	{
		if (cmd[index - 1].redir[0] != FILE)
			safeclose(garb[index - 1].pipefd[0]);
	}
}

/* waits all the pids and returns the exit code of the last child.
NOTE: The father is always listening to his childrens :) */
static int	mass_wait(t_garb *garb, size_t len)
{
	unsigned int	i;
	int				exit_code;

	i = 0;
	while (i < (len - 1))
	{
		if (garb[i].pid > 0)
		waitpid(garb[i].pid, NULL, WUNTRACED);
		i++;
	}
	exit_code = 0;
	waitpid(garb[i].pid, &exit_code, WUNTRACED);
	i = 0;
	while (i < len) /* AWWWWWWW SOOOO CUTEEE :3 */
	{
		safeclose(garb[i].pipefd[0]);
		safeclose(garb[i].pipefd[1]);
		i++;
	}
	return (((exit_code) & 0xff00) >> 8);
}

// static void print_garbage(t_garb *garb, size_t len)
// {
// 	unsigned int i;

// 	i = 0;
// 	while (i < len)
// 	{
// 		ft_printf("= == GARB[%d] === =\n", i);
// 		ft_printf("\tpipefd[%d, %d]\n", garb[i].pipefd[0], garb[i].pipefd[1]);
// 		ft_printf("\t    fd[%d, %d]\n", garb[i].fd[0], garb[i].fd[1]);
// 		i++;
// 	}
// }

/* TEST:
<main.c grep main | cat >out | ls | <main.c cat >out2 | pwd */

/* Executes a single command */
static void	single_execute(int *execfd, int index, t_cmd *cmd, char ***vars)
{
	int	exit_status;
	
	if (ft_strichr(cmd[index].words[0], '=') != 0)
	{
		if (cmd[index].words[1] != NULL)
			cmd[index] = pop_arg(cmd[index], 0);
		else
		{
			handle_vars(cmd[index], vars);
			clean_exit(execfd, cmd, vars, EXIT_SUCCESS);
		}
	}
	if (!is_builtin(cmd[index].words[0]))
	{
		exit_status = ft_execve(execfd, cmd[index], vars[1] + 1);
		clean_exit(execfd, cmd, vars, exit_status);
	}
	// if (!ft_strncmp("exit", cmd[index].words[0], 5))
	// 	clean_exit(execfd, cmd, vars, CLEAN_EXIT);
	// safeclose(execfd[0]);
	builtin_and_die(execfd, index, cmd, vars);
}

int	execute_pipeline(char *line, t_cmd *cmd, char ***vars)
{
	t_garb			*garb;
	size_t			len;
	unsigned int	i;
	int				execfd[2];
	int				exit_status;

	len = ft_cmdlen(cmd);
	if (len == 0)
		return (1);
	garb = (t_garb *)ft_calloc(len, sizeof(t_garb));
	if (garb == NULL)
	{
		write(STDERR_FILENO, "minishell: malloc failure\n", 26);
		return (1);
	}
	i = 0;
	while (i < len)
	{
		execfd[0] = redir_input(cmd, garb, i, len);
		execfd[1] = redir_output(cmd, garb, i, len);
		garb[i].pid = fork();
		if (garb[i].pid < 0)
		{
			ft_printfd(STDERR_FILENO, "minishell: %s: fork failure\n", cmd[i].words[0]);
			father_closes(cmd, garb, i, len);
			exit_status = mass_wait(garb, i + 1);
		//	print_garbage(garb, len);
			free(garb);
			return (exit_status);
		}
		if (garb[i].pid == 0) //child
		{
			/* Nel figlio ripristina il comportamento di default per SIGINT e SIGQUIT */
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			child_closes(cmd, garb, i, len);
			free(garb); free(line);
			single_execute(execfd, i, cmd, vars);
		}
		father_closes(cmd, garb, i, len);
		i++;
	}
	exit_status = mass_wait(garb, len);
//	print_garbage(garb, len);
	free(garb);
	return (exit_status);
}


		// if (cmd_arr[i].fd[0] != 0 && i != 0)
		// {
		// 	cleanclose(&garb[i - 1].pipefd[0], INPUT);
		// 	execfd[0] = cmd_arr[i].fd[0];
		// }
		// if (cmd_arr[i].fd[1] != 0)
		// {
		// 	garb[i].pid = fork();
		// 	if (cmds[index].pid < 0)
		// 	{
		// 		write(STDERR_FILENO, "fork failure\n", 15);
		// 		//
		// 		// ...or just return
		// 		return (0);
		// 	}
		// 	ft_execve(garb[i].pipefd, execfd, env);
		// }
		// if (pipe_and_fork(garb, i) < 0)
		// {
		// 	i++;
		// 	continue;
		// 	// ...or just return
		// }
		// if (garb[i].pid == 0)
		// {
		// 	//close(garb[i].pipefd[0]);
		// 	cleanclose(&garb[i].pipefd[0], INPUT);
		// 	ft_execve(garb[i].pipefd, cmd_arr[i], env);
		// }
		// if (garb[i].pid != 0)
		// {
		// 	cleanclose(&garb[i].pipefd[0], INPUT);
		// 	//close(garb[i].pipefd[1]);
		// }