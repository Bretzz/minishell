/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 18:58:04 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/08 21:24:52 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_readline.h"
#include "libft.h"

int	is_end_of_trans(char *input);
int	is_arrow_key(char *input);
int	is_delete_key(char *input);
int	is_newline(char *input);

#define UP_ARROW 'A'
#define DOWN_ARROW 'B'
#define RIGHT_ARROW 'C'
#define LEFT_ARROW 'D'
#define DELETE_KEY 0x7F

/* RETURNS:
	UP_ARROW 	1
	DOWN_ARROW -1'
	RIGHT_ARROW 2
	LEFT_ARROW -2
	NOT_ARROW	0 */
int	is_arrow_key(char *input)
{
	if (!input || input[0] != 0x1b || input[1] != '[')
		return (0);
	if (input[2] == UP_ARROW)
		return (1);
	else if (input[2] == DOWN_ARROW)
		return (-1);
	else if (input[2] == RIGHT_ARROW)
		return (2);
	else if (input[2] == LEFT_ARROW)
		return (-2);
	else
	return (0);
}

int	is_end_of_trans(char *input)
{
	if (input && input[0] == 0x4) // Ctrl+D (End-of-Transmission)
	{
		write(STDOUT_FILENO, "Ctrl+D Pressed (EOF)\n", 21);
		//write(STDOUT_FILENO, "\n", 1);
		return (1);
	}
	return (0);
}

int	is_delete_key(char *input)
{
	if (input && input[0] == DELETE_KEY) // Ctrl+D (End-of-Transmission)
		return (1);
	return (0);
}

int	is_newline(char *input)
{
	if (*input == '\n')
		return (1);
	return (0);
}

int	handle_escape_sequence(char *input)
{	
	if (input[1] != '[')
		return (0);
	if (input[2] == UP_ARROW)
		;
		//write(STDOUT_FILENO, "Up Arrow\n", 9);
	else if (input[2] == DOWN_ARROW)
		;
		//write(STDOUT_FILENO, "Down Arrow\n", 11);
	else if (input[2] == RIGHT_ARROW)
		;
		//write(STDOUT_FILENO, "Right Arrow\n", 12);
	else if (input[2] == LEFT_ARROW)
		;
		//write(STDOUT_FILENO, "Left Arrow\n", 11);
	else
		write(STDOUT_FILENO, "Unknown Escape Sequence\n", 24);
	return (0);
}

// Function to handle special key sequences
int	handle_special_keys(char *input)
{
	static int	typed;

	if (input == NULL)
		return (0);
	//write(STDOUT_FILENO, "handling something special >:)\n", 32);
	if (input[0] == 0x1b)
		/* handle_escape_sequence(input) */;
	else if (input[0] == 0x4) // Ctrl+D (End-of-Transmission)
	{
		write(STDOUT_FILENO, "Ctrl+D Pressed (EOF)\n", 21);
		//write(STDOUT_FILENO, "\n", 1);
		return (0);
	}
	else if (input[0] == 0x7F) // Delete key (ASCII 0x7F)
	{
		// write(STDOUT_FILENO, "Delete Key Pressed\n", 19);
		if (typed != 0)
		{
			write(STDOUT_FILENO, "\b \b", 3);
			typed--;
		}
	}
	else
	{
		write(STDOUT_FILENO, input, strlen(input));
		typed++;
	}
	return (1);
}
