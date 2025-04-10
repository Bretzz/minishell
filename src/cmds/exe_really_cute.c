/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_really_cute.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totommi <totommi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:54:29 by totommi           #+#    #+#             */
/*   Updated: 2025/04/10 14:59:54 by totommi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "cmds.h"

#define INPUT 0 /* Reassign of default INPUT value to closed fd. */
#define OUTPUT 1 /* Reassign of default OUTPUT value to closed fd. */

int		mass_wait(t_garb *garb, size_t len);
void	father_closes(t_cmd *cmd, t_garb *garb, int index, size_t len);
void	child_closes(t_cmd *cmd, t_garb *garb, int index, size_t len);

/* This part of the code expects an array (2+) of cmds all separated by pipes.
if there are multiple commands it links them in pipes.
NOTE: All variables are already expanded,
all heredocs filled and files (read/write) opened. */
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

/* Closes all the fds of the files opened for the next command(s) in line.
Then closes all the fds of the previous 'hanging pipes',
ex: cat | <file grep "str". */
static void	carpet_close(t_cmd *cmd, t_garb *garb, int index, size_t len)
{
	unsigned int	i;

	i = index + 1;
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
void	father_closes(t_cmd *cmd, t_garb *garb, int index, size_t len)
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
void	child_closes(t_cmd *cmd, t_garb *garb, int index, size_t len)
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
NOTE: The father is always listening to his childrens :3
AWWWWWWW SOOOO CUTEEE :3 */
int	mass_wait(t_garb *garb, size_t len)
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
	exit_code = ft_wifexited(garb[i].pid);
	i = 0;
	while (i < len)
	{
		safeclose(garb[i].pipefd[0]);
		safeclose(garb[i].pipefd[1]);
		i++;
	}
	return (exit_code);
}
