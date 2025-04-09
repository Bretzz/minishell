/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   what_is_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 18:58:04 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/09 15:23:08 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_readline.h"
#include "libft.h"

int			is_end_of_trans(char *input);
int			is_arrow_key(char *input);
int			is_one_of_the_arrows(t_esc_type esc_seq);
int			is_delete_key(char *input);
int 		is_escape_sequence(char *input);
int			is_newline(char *input);
t_esc_type	input_is(char *input);

#define UP_ARROW 'A'
#define DOWN_ARROW 'B'
#define RIGHT_ARROW 'C'
#define LEFT_ARROW 'D'
#define DELETE 0x7F
#define ESCAPE 0x1b
#define EOT	0x4

t_esc_type	input_is(char *input)
{
	t_esc_type arr_key;

	arr_key = is_arrow_key(input);
	if (input == NULL)
		return (NULL_INPUT);
	if (is_end_of_trans(input))
		return (EOT_SEQ);
	if (is_arrow_key(input))
		return (arr_key);
	if (is_delete_key(input))
		return (DEL_SEQ);
	if (is_escape_sequence(input))
		return (ESC_SEQ);
	else
		return (NOT_SEQ);
}

int	is_one_of_the_arrows(t_esc_type esc_seq)
{
	if (esc_seq == ARR_U_SEQ
		|| esc_seq == ARR_D_SEQ
		|| esc_seq == ARR_R_SEQ
		|| esc_seq == ARR_L_SEQ)
		return (1);
	return (0);
}

int	is_arrow_key(char *input)
{
	if (!input || input[0] != ESCAPE || input[1] != '[')
		return (0);
	if (input[2] == UP_ARROW)
		return (ARR_U_SEQ);
	else if (input[2] == DOWN_ARROW)
		return (ARR_D_SEQ);
	else if (input[2] == RIGHT_ARROW)
		return (ARR_R_SEQ);
	else if (input[2] == LEFT_ARROW)
		return (ARR_L_SEQ);
	else
		return (0);
}

int	is_end_of_trans(char *input)
{
	if (input && input[0] == EOT)
	{
		write(STDOUT_FILENO, "Ctrl+D Pressed (EOF)\n", 21);
		return (1);
	}
	return (0);
}

int	is_delete_key(char *input)
{
	if (input && input[0] == DELETE)
		return (1);
	return (0);
}

int	is_newline(char *input)
{
	if (*input == '\n')
		return (1);
	return (0);
}

int is_escape_sequence(char *input)
{
	if (input[0] == ESCAPE && input[1] != '\0')
		return (1);
	return (0);
}
