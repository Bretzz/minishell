/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_line.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 13:23:42 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/10 00:04:06 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_readline.h"
#include "libft.h"

char	*build_line(int fd, char **history);

/* Overwrites the 'typed' char of the history line we are on with a '\0',
then replace the last char writtend on STDOUT with a blank space,
then reduced the value of typed by one. */
static void	delete_one_char(size_t *cursor, char **history, int hist_line)
{
	if (*cursor > 0)
	{
		do_delete_key(1, *cursor);
		(*cursor)--;
	}
	history[hist_line][*cursor] = '\0';
}

/* based on the 'esq_seq' passed execute the corresponding operations.
RETURNS: 1 if you can 'countinue', 0 if you should return NULL. */
static int	handle_esc_seq(t_esc_type esc_seq, size_t *cursor, char **history, int *hist_line)
{
	if (esc_seq == NULL_INPUT || esc_seq == EOT_SEQ)
	{
		return (0);
	}
	if (esc_seq == ARR_U_SEQ
		|| esc_seq == ARR_D_SEQ
		|| esc_seq == ARR_R_SEQ
		|| esc_seq == ARR_L_SEQ)
	{
		use_arrow_key(esc_seq, cursor, history, hist_line);
		return (1);
	}
	if (esc_seq == DEL_SEQ)
	{
		delete_one_char(cursor, history, *hist_line);
		return (1);
	}
	if (esc_seq == ESC_SEQ)
	{
		return (1);
	}
	return (-1);	//not handled
}

static char	*add_char(unsigned int cursor, char *line, char *new_char)
{
	size_t	len;
	char	*new_line;

	len = ft_strlen(line);
//	ft_printf("cursor %i, len %i\n", cursor, len);
	if (cursor == len)
		new_line = ft_strjoin_free(line, new_char);
	else if (cursor < len)
	{
		new_line = (char *)ft_realloc(line, (len + 1) * sizeof(char) , (len + 2) * sizeof(char));
		if (new_line == NULL)
			return (NULL);
		new_line[len + 1] = '\0';
		ft_memmove(new_line + cursor + 1, new_line + cursor, ft_strlen(new_line + cursor));
		new_line[cursor] = *new_char;
		//new_line[len] = '\0';
//		ft_printf("new_line '%s'\n", new_line);
	}
	else
		new_line = line;
	return (new_line);
}

static int	handle_std_char(char *input, size_t *cursor, char **history, int hist_line)
{
	size_t	typed;

	typed = ft_strlen(history[hist_line]);
	if (is_newline(input))
	{
		write(STDOUT_FILENO, input, 1);
		*cursor = 0;
		if (*history[hist_line] == '\0')
			return (0);
		return (1);
	}
	else
	{
		history[hist_line] = add_char(*cursor, history[hist_line], input);
		(*cursor)++;
		if (history[hist_line] == NULL)
			write(STDERR_FILENO, "ft_readline: malloc failure\n", 28);
		else
			ft_redisplay(typed, *cursor, history[hist_line]);
	}
	return (1);
}

/* Reads the input from the terminal byte by byte,
handling eventual escape sequence with handle_special_keys().
RETURNS: the input read (up to '\n'), NULL on SIGINT or malloc failure. */
char	*build_line(int fd, char **history)
{
	struct termios	old_tio;
	t_esc_type		esc_seq;
	char			*input;
	int				hist_line;
	size_t			cursor;

	hist_line = 0;
	cursor = 0;
	set_terminal_mode(&old_tio);
	history[hist_line] = ft_strdup("");
	while (history[hist_line] != NULL)
	{
		input = ft_getchar(fd);
		esc_seq = input_is(input);
	//	ft_printf("esc_sec %d\n", esc_seq);
		if (esc_seq != NOT_SEQ)
		{
			free(input);
			if (handle_esc_seq(esc_seq, &cursor, history, &hist_line))
				continue ;
			break ;
		}
		if (!handle_std_char(input, &cursor, history, hist_line))
			return (free(input),
				restore_terminal_mode(&old_tio),
				history[hist_line]);
		else if (is_newline(input))
			return (free(input),
				restore_terminal_mode(&old_tio),
				ft_strdup(history[hist_line]));
		free(input);
	}
	restore_terminal_mode(&old_tio);
	return (NULL);
}
