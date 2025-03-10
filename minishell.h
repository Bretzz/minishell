/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 16:05:55 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/10 20:00:50 by topiana-         ###   ########.fr       */
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

int		handle_vars(char *cmd, char ***shv, char **env);
char	*get_value(const char *target, const char **shv, const char **env);
char	*expand_string(char *str, char **shv, char **env);

//ft_execve.c

int		ft_execve(int *fd, const char *cmd, char **env);

//built_ins.c

int		ft_echo(char *cmd, char **shv, char **env);
int		ft_cd(char *cmd);
int		ft_pwd(char *cmd);

//free_stuff.c

void	ft_free_charr(char **arr);
void	ft_free_arr(void **arr);

//usefull stuff

char	*ft_strjoin_free_space_nl(char *s1, char *s2);

void	*ft_realloc(void *ptr, size_t old_size, size_t new_size);
size_t	ft_strlen_nl(const char *s);
size_t	ft_strlen_space(const char *s);
size_t	ft_mtxlen(const void **mtx);

void	ft_print_charr(const char **arr);

#endif