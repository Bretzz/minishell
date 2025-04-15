/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_readline.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 13:48:38 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/09 16:21:38 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_READLINE_H
# define FT_READLINE_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <termios.h>
# include <fcntl.h>
# include <sys/ioctl.h>
# include <string.h>

# define MAX_HISTORY 42

typedef enum e_esc_type
{
	EOT_SEQ,
	ARR_SEQ,
	ARR_U_SEQ,
	ARR_D_SEQ,
	ARR_R_SEQ,
	ARR_L_SEQ,
	DEL_SEQ,
	ESC_SEQ,
	NOT_SEQ,
	NULL_INPUT,
}			t_esc_type;

/* ======= TO EXPORT ======== */

/* BETTER READLINE */

char		*ft_readline(const char *prompt);

/* HISTORY */

char		**ft_add_history(char *line);
void		ft_free_history(void);

/* ====== INTERNAL USE ======= */

/* GETCHAR */

char		*ft_getchar(int fd);

/* BUILD LINE */

char		*build_line(int fd, char **history);

/* TERMIOS SETUP */

void		set_terminal_mode(struct termios *old_tio);
void		restore_terminal_mode(struct termios *old_tio);
int			is_input_ready(int fd);

/* WHAT IS THE INPUT? */

int			is_end_of_trans(char *input);
int			is_arrow_key(char *input);
int			is_one_of_the_arrows(t_esc_type esc_seq);
int			is_delete_key(char *input);
int 		is_escape_sequence(char *input);
int			is_newline(char *input);
t_esc_type	input_is(char *input);

/* KEYS DO STUFF */

void		do_delete_key(int typed, int cursor);
void		use_arrow_key(t_esc_type arr_key, size_t *cursor, char **history, int *hist_line);

void 		ft_redisplay(int typed, int cursor, char *new_line);

#endif