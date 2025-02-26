/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 16:05:55 by topiana-          #+#    #+#             */
/*   Updated: 2025/02/26 20:42:23 by topiana-         ###   ########.fr       */
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

//ft_readline.c

char	*ft_readline (const char *prompt);

//pipex_stolen.c

char	*find_env_path(char *cmd, char **env);

//ft_execve.c

int		ft_execve(int *fd, const char *cmd, char **env);

//built_ins.c

int		ft_echo(char *cmd);

//free_stuff.c

void	ft_free_arr(char **arr);

#endif