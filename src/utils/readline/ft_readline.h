/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_readline.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 13:48:38 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/08 21:25:00 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_READLINE_H
# define FT_READLINE_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <string.h>

#define MAX_HISTORY 42

/* BETTER READLINE */

char	*ft_readline(const char *prompt);

/* HISTORY */

void	ft_add_history(char *line);
char	**ft_get_history(char *history);
void	ft_set_history(char **history);
void	ft_free_history(void);

/* GETCHAR */

char	*ft_getchar(int fd);

/* TERMIOS SETUP */

void	set_terminal_mode(struct termios *old_tio);
void	restore_terminal_mode(struct termios *old_tio);
int		is_input_ready(int fd);

/* HANDLE INPUT */

int		handle_special_keys(char *input);
int		is_end_of_trans(char *input);
int		is_arrow_key(char *input);
int		is_delete_key(char *input);
int		is_newline(char *input);

#endif