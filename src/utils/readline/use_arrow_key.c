/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   use_arrow_key.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 13:59:18 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/09 17:37:48 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_readline.h"
#include "libft.h"

void	use_arrow_key(t_esc_type arr_key, size_t *cursor, char **history, int *hist_line);
void 	ft_redisplay(int typed, int cursor, char *new_line);

/* Removes 'typed' chars from the STDOUT and writes the 'new_line'.
the value pointed py 'typed' is changed to the length of the new_line. */
void ft_redisplay(int typed, int cursor, char *new_line)
{
	size_t	len;
	int		diff;

	len = ft_strlen(new_line);
	do_delete_key(typed, cursor);
	write(STDOUT_FILENO, new_line, len);
	diff = len - cursor;
	while (diff > 0)
	{
		//write(STDOUT_FILENO, "\b", 1);
		diff--;
	}
	//ft_printf("cursor: %d, len %d\n", cursor, len);
}

/* Based on the arrow_keu passed, changles the value inside the 'hist_line' passed.
arrow_key values are the one returned by the is_arrow_key() function.
RETURNS: 1 if 'hist_line' has changed, 0 otherwise. */
static int cycle_history(int arrow_key, char **history, int *hist_line)
{
	if (arrow_key != 0)
	{
		if (*hist_line + arrow_key < 0
			|| *hist_line + arrow_key > MAX_HISTORY
			|| history[*hist_line + arrow_key] == NULL
			|| arrow_key % 2 == 0)
			return (0);
		*hist_line += arrow_key;
		return (1);
	}
	return (0);
}

static void	do_LR_arrow_key(t_esc_type arr_key, size_t *cursor, char **history, int hist_line)
{
	if (arr_key == ARR_L_SEQ)
	{
		if (cursor > 0)
		{
			write(STDOUT_FILENO, "\b", 1);
			(*cursor)--;
		}
	}
	if (arr_key == ARR_R_SEQ)
	{
		if (*cursor < ft_strlen(history[hist_line]))
		{
			(*cursor)++;
			write(STDOUT_FILENO, "\f", 1);
		}
	}
}

/* cycles trough history and updates the displayed line */
static void	do_UP_arrow_key(t_esc_type arr_key, size_t *cursor, char **history, int *hist_line)
{
	size_t	typed;

	typed = ft_strlen(history[*hist_line]);
	if (arr_key == ARR_U_SEQ)
		cycle_history(1, history, hist_line);
	else
		cycle_history(-1, history, hist_line);
	ft_redisplay(typed, *cursor, history[*hist_line]);
	*cursor = ft_strlen(history[*hist_line]);
}

void	use_arrow_key(t_esc_type arr_key, size_t *cursor, char **history, int *hist_line)
{
	if (arr_key == ARR_U_SEQ || arr_key == ARR_D_SEQ)
	{
		do_UP_arrow_key(arr_key, cursor, history, hist_line);
	}
	else if (arr_key == ARR_R_SEQ || arr_key == ARR_L_SEQ)
	{
		do_LR_arrow_key(arr_key, cursor, history, *hist_line);
	}
}
