/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totommi <totommi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 12:15:55 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/05 15:23:42 by totommi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMDS_H
# define CMDS_H

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

//pipex_stolen.c

int		ft_execve(int *fd, t_cmd cmd, char **env);
char	*find_env_path(char *cmd, char **env);

#endif