/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 16:05:55 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/10 14:42:41 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <term.h>

# include <readline/readline.h>
# include <readline/history.h>

typedef struct s_var
{
	char	*name;
	char	*value;
}				t_var;

//ft_readline.c

char	*ft_readline (const char *prompt);

//pipex_stolen.c

char	*find_env_path(char *cmd, char **env);

//handle stuff

void	handle_vars(char *cmd, char ***sh_vars, char **__environ);

//ft_execve.c

int		ft_execve(int *fd, const char *cmd, char **env);

//built_ins.c

int		ft_echo(char *cmd);
int		ft_cd(char *cmd);
int		ft_pwd(char *cmd);

//free_stuff.c

void	ft_free_charr(char **arr);
void	ft_free_arr(void **arr);

//usefull stuff

void	*ft_realloc(void *ptr, size_t old_size, size_t new_size);
char	*ft_strjoin_free_space_nl(char *s1, char *s2);
size_t	ft_strlen_nl(const char *s);
size_t	ft_mtxlen(const void **mtx);
void	ft_print_charr(const char **arr);

#endif