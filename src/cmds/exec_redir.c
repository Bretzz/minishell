/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 15:35:27 by totommi           #+#    #+#             */
/*   Updated: 2025/04/16 16:20:42 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "cmds.h"

int		pipeline_redir_input(t_cmd *cmd, t_garb *garb, int index, size_t len);
int		pipeline_redir_output(t_cmd *cmd, t_garb *garb, int index, size_t len);
int		command_redir_input(t_cmd *cmd);
int		command_redir_output(t_cmd *cmd);

/* returns the fd where to run execve.
If the cmd at 'index' already has a preferred input closes the previous
read-end of the pipe, then returns the fd, otherwise returns the read-end
of the previous pipe.
If it's the first command and no preferred fd
is specified, returns STDIN_FILENO.
NOTE: with 'preferred fd' we are refering to the fd in the t_cmd struct */
int	pipeline_redir_input(t_cmd *cmd, t_garb *garb, int index, size_t len)
{
	(void)len;
	if (cmd[index].redir[0] == FILE || cmd[index].redir[0] == HERE_DOC)
	{
		return (cmd[index].fd[0]);
	}
	if (index == 0)
	{
		return (STDIN_FILENO);
	}
	return (garb[index - 1].pipefd[0]);
}

/* If the cmd at 'index' has a preferred ouput just returns it, 
otherwise creates a new pipe and returns the write-end of that pipe.
If we are at the last command and no preferred fd
is specified, returns STDOUT_FILENO.
NOTE: with 'preferred fd' we are refering to tho the fd in the t_cmd struct */
int	pipeline_redir_output(t_cmd *cmd, t_garb *garb, int index, size_t len)
{
	if (index == (int)(len - 1))
	{
		if (cmd[index].redir[1] == FILE && cmd->fd[1] != -1)
		{
			cmd[index].fd[1] = open(cmd[index].outfile,
					cmd[index].append, 0644);
			return (cmd[index].fd[1]);
		}
		return (STDOUT_FILENO);
	}
	if (pipe(garb[index].pipefd) < 0)
	{
		garb[index].pipefd[0] = STDIN_FILENO;
		garb[index].pipefd[1] = STDOUT_FILENO;
		write(STDERR_FILENO, "minishell: pipe failure\n", 24);
	}
	if (cmd[index].redir[1] == FILE)
	{
		cmd[index].fd[1] = open(cmd[index].outfile, cmd[index].append, 0644);
		return (cmd[index].fd[1]);
	}
	return (garb[index].pipefd[1]);
}

/* takes a cmd pointer as parameter
Based on that returns the fd on which
the command should read. */
int	command_redir_input(t_cmd *cmd)
{
	if (cmd->redir[0] == FILE || cmd->redir[0] == HERE_DOC)
		return (cmd->fd[0]);
	if (cmd->redir[0] == PIPE)
		return (dummy_in_pipe());
	return (STDIN_FILENO);
}

/* takes a cmd pointer as parameter
Based on that returns the fd on which
the command should write,
CLEAN_EXIT (-1) on pipe(3) failure. */
int	command_redir_output(t_cmd *cmd)
{
	int	pipefd[2];

	if (cmd->redir[1] == FILE && cmd->fd[1] != -1)
	{
		cmd->fd[1] = open(cmd->outfile, cmd->append, 0644);
		return (cmd->fd[1]);
	}
	if (cmd->redir[1] == PIPE)
	{
		pipefd[0] = STDIN_FILENO;
		pipefd[1] = STDOUT_FILENO;
		if (pipe(pipefd) < 0)
		{
			write(STDERR_FILENO, "minishell: pipe failure\n", 24);
			return (CLEAN_EXIT);
		}
		else
		{
			cmd->close_me = pipefd[0];
			return (pipefd[1]);
		}
	}
	return (STDOUT_FILENO);
}
