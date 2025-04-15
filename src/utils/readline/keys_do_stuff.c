/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys_do_stuff.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 13:36:22 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/09 17:54:27 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_readline.h"
#include "libft.h"

void	do_delete_key(int typed, int cursor);

/* static  */void	cursor_forward(int typed, int cursor)
{
	if (cursor == typed)
		return ;
	while (cursor < typed)
	{
		write(STDOUT_FILENO, " ", 1);
		cursor++;
	}
}

void	do_delete_key(int num, int cursor)
{
	int	i;

	(void)cursor;
	i = 0;
	while (i < num)
	{
		write(STDOUT_FILENO, "\b", 1);
		i++;
	}
	i = 0;
	while (i < num)
	{
		write(STDOUT_FILENO, " ", 1);
		i++;
	}
	i = 0;
	while (i < num)
	{
		write(STDOUT_FILENO, "\b", 1);
		i++;
	}
}
