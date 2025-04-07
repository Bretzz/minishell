/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 12:15:55 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/07 17:39:18 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMDS_H
# define CMDS_H

#define CREATE 0 /* open_doc() creation flag. */
#define RESET 1	/* reset the doc_num static variable. */
#define GETNUM 2 /* open_doc() information flag. */

typedef struct s_garb
{
	pid_t	pid;
	int		fd[2];
	int		pipefd[2];
}			t_garb;

/* BUILTIN BRIDGE (BB) */

int		is_builtin(char *cmd);
int		exec_builtin(int *fd, t_cmd cmd, char ***vars);
void	builtin_and_die(int *fd, int index, t_cmd *cmd_arr, char ***vars);

/* HERE_DOC UTILS */

int		open_doc(char flag);
void	close_docs(void);
int		read_doc(int doc_num);
char	*get_doc_path(int doc_num, char *buff, size_t size);

//pipex_stolen.c

int		ft_execve(int *fd, t_cmd cmd, char **env);
char	*find_env_path(char *cmd, char **env);

//dummy pipes

int dummy_in_pipe(void);

#endif