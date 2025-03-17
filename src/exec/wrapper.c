/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrapper.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 20:38:09 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/17 21:22:08 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

pid_t	wrapper(int *fd, t_cmd cmd);

/* takes a fd pointer to a stack allocated mem of size 2 (int fd[2]),
a cmd struct, the shell variables and the enviroment variables as parameters.
It forks, then retuns the pid of the child (in the father),
then calls dup2(3) on the redirections (or throws errors) for the child.
RETURNS: the pid of the child (father) or 0 (child). */
pid_t	wrapper(int *fd, t_cmd cmd)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (-1); //send error
	else if (pid != 0)
		return (pid);

	fd[0] = 0;
	if (ft_strncmp("|", cmd.infile, 2) && ft_strncmp("<<", cmd.infile, 3)) //don't handle for now
	{
		if (cmd.infile[0] != '\0')
		{
			if (access(cmd.infile, R_OK) == 0)
				fd[0] = open(cmd.infile, O_RDONLY);
			else
				ft_printfd(2, "minishell: can't open file or it doesn't exist, dunno yet\n"); //handle error
		}
	}
	
	fd[1] = 1;
	if (ft_strncmp("|", cmd.outfile, 2)) //don't handle for now
	{
		if (cmd.outfile[0] != '\0')
		{
			fd[1] = open(cmd.outfile, cmd.append, 0644);
			if (fd[1] < 0)
			{
				ft_printfd(2, "minishell: can't open file or it doesn't exist, dunno yet\n"); //handle error
				fd[1] = 1;
			}
		}
	}
	dup2(fd[0], STDIN_FILENO);
	dup2(fd[1], STDOUT_FILENO);
	return (pid);
}
