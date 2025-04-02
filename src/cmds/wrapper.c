/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrapper.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 20:38:09 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/02 22:25:56 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

pid_t	wrapper(int *fd, int *oldfd, t_cmd cmd);
int		miniwrapper(int *fd, int *oldfd, t_cmd cmd);

/* takes argv and a pipedes as parameters.
writes "pipe heredoc> " to the stdout,
writes the stdin to the write end of the pipe,
breaks when the stdin is exactly argv[2] */
// static void	here_doc(int where, char *limiter, int *pipefd)
// {
// 	char	*line;
// 	char	prompt[15];

// 	ft_bzero(prompt, 15);
// 	if (where == PIPE)
// 		ft_strlcpy(prompt, "pipe heredoc> ", 15);
// 	else
// 		ft_strlcpy(prompt, "heredoc> ", 10);
// 	ft_printf("%s", prompt);
// 	line = get_next_line(STDIN_FILENO);
// 	while (/* line != NULL &&  */!(line && !ft_strncmp(limiter, line, ft_strlen(limiter))
// 			&& line[ft_strlen(limiter)] == '\n'))
// 	{
// 		if (line != NULL)
// 		{
// 			write(pipefd[1], line, ft_strlen(line));
// 			free(line);
// 		}
// 		else
// 			ft_printf("\nminishell: is weak...\n");
// 		ft_printf("%s", prompt);
// 		line = get_next_line(STDIN_FILENO);
// 	}
// 	//ft_printf("\n");
// 	free(line);
// 	close(pipefd[1]);
// }

/* defaults returns STDIN_FILENO,
if an input file is found, returns it's fd after a successful open(3) call. */
static int	redir_input(int *oldfd, t_cmd cmd)
{
	//int	pipefd[2];
	
	//ft_printf("infile:%s, redir=%d\n", cmd.infile, cmd.redir[0]);
	if (cmd.redir[0] == PIPE)
	{
		return (oldfd[0]);
	}
	// if (cmd.redir[0] == HERE_DOC)
	// {
	// 	if (pipe(pipefd) < 0)
	// 	{
	// 		ft_printfd(2, "minishell: pippe error to be handled\n");
	// 		return (-1);
	// 	}
	// 	if (cmd.redir[1] == PIPE)
	// 		here_doc(PIPE, cmd.infile, pipefd);
	// 	else
	// 		here_doc(STDL, cmd.infile, pipefd);
	// 	return (pipefd[0]);
	// }
	if (cmd.redir[0] == FILE || cmd.redir[0] == HERE_DOC)
	{
		safeclose(oldfd[0]);
		return (cmd.fd[0]);
		// if (access(cmd.infile, R_OK) == 0)
		// 	return(open(cmd.infile, O_RDONLY));
		// return (-1);
	}
	return (STDIN_FILENO);
}

/* defaults returns STDIN_FILENO,
if an output file is found, returns it's fd after a successful open(3) call. */
static int	redir_output(int *pipefd, t_cmd cmd)
{
	//int	pipefd[2];
	
	//ft_printf("outfile:'%s', redir=%d\n", cmd.outfile, cmd.redir[1]);
	if (cmd.redir[1] == PIPE)
	{
		if (pipe(pipefd) < 0)
		{
			ft_printfd(2, "minishell: pippe error to be handled\n");
			return (-1);
		}
		/* oldfd[0] = pipefd[0];
		oldfd[1] = pipefd[1]; */
		return (pipefd[1]);
	}
	if (cmd.redir[1] == FILE)
	{
		return (cmd.fd[1]);
		// //if (access(cmd.outfile, W_OK) == 0)
		
		// if (access(cmd.outfile, F_OK) == -1 || access(cmd.outfile, W_OK) == 0)
		// 	return(open(cmd.outfile, cmd.append, 0644));
		// return (-1);
	}
	return (STDOUT_FILENO);
}

/* takes a fd pointer to a stack allocated mem of size 2 (int fd[2]),
and a cmd struct as parameters.
Assign the redirected input and output to fd[0] and fd[1] respectively,
and copies pipefd into oldfd (if we piped).
It forks, then 
FATHER:
	the FATHER close no-needed fds,
	then returns the pid of the child,
CHILD:
	the CHILD closes no-needed fds then 
	calls dup2(3) on the redirections (or throws errors).
RETURNS: the pid of the child (father) or 0 (child). */
pid_t	wrapper(int *fd, int *oldfd, t_cmd cmd)
{
	pid_t		pid;
	//static int	oldfd[2]; // a copy of the previous pipe
	int			pipefd[2]; // fds of the current pipe

	pipefd[0] = 0;
	pipefd[1] = 1;
	fd[0] = redir_input(oldfd, cmd);
	fd[1] = redir_output(pipefd, cmd);
	// ft_printf("----oldfd=[%d,%d]\n", oldfd[0], oldfd[1]);
	// ft_printf("---pipefd=[%d,%d]\n", pipefd[0], pipefd[1]);
	pid = fork();
	if (pid < 0)
		return (-1); //send error
	else if (pid != 0)
	{
		if (cmd.redir[0] == PIPE) // the father doesn't need the READ end of the pipe since just the child's gonna read on it
		{
			// ft_printf("----father closed %d\n", oldfd[0]);
			close(oldfd[0]);
		}
		if (cmd.redir[1] == PIPE) // the father doesn't need the WRITE end of the pipe since the child's gonna write on it
		{
			// ft_printf("----father closed %d\n", pipefd[1]);
			close(pipefd[1]);
		}
		if (cmd.redir[1] == FILE && fd[1] > 2)
			close(fd[1]);
		if ((cmd.redir[0] == FILE || cmd.redir[0] == HERE_DOC) && fd[0] > 2)
			close(fd[0]);
		ft_memcpy(oldfd, pipefd, 2 * sizeof(int)); // backup of old pipe
		return (pid);
	}
	// if (cmd.redir[0] == PIPE) // the father doesn't need the READ end of the pipe since jsut the child's gonna read on it
	// 	close(oldfd[1]);
	if (cmd.redir[1] == PIPE) // the child doesn't need the READ end of the pipe since he's just gonna write on it
	{
		// ft_printf("----child closed %d\n", pipefd[0]);
		close(pipefd[0]);
	}
	/* dup2(fd[0], STDIN_FILENO);
	dup2(fd[1], STDOUT_FILENO); */
	// no need for backups, he's gonna die anyway
	return (pid);
}

int	miniwrapper(int *fd, int *oldfd, t_cmd cmd)
{
	int	pipefd[2]; // fds of the current pipe

	pipefd[0] = 0;
	pipefd[1] = 1;
	fd[0] = redir_input(oldfd, cmd);
	fd[1] = redir_output(pipefd, cmd);
	// ft_printf("----oldfd=[%d,%d]\n", oldfd[0], oldfd[1]);
	// ft_printf("---pipefd=[%d,%d]\n", pipefd[0], pipefd[1]);
	// ft_printf("--finalfd=[%d,%d]\n", fd[0], fd[1]);

	ft_memcpy(oldfd, pipefd, 2 * sizeof(int));
	return (0);
}
