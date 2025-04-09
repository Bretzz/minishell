/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getchar.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 14:23:01 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/08 18:57:53 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_readline.h"
#include "libft.h"

char	*ft_getchar(int fd);
int		handle_special_keys(char *input);
int		handle_arrows(char *input);

static void	*null_return(char *buff)
{
	write(STDERR_FILENO, "ft_getchar: read failure\n", 25);
	free(buff);
	return (NULL);
}

/* Reads a single char from fd, then returns the buffer.
NOTE: if an escape sequence is sent, further read(3) calls are
made until the whole sequence is stored in buff. */
char *ft_getchar(int fd)
{
	char				*buff;
	ssize_t				bytes_read;
	unsigned int		idx;

	buff = (char *)ft_calloc(10, sizeof(char));
	if (buff == NULL)
	{
		write(STDERR_FILENO, "ft_getchar: malloc failure\n", 27);
		return (NULL);
	}
	idx = 0;
	bytes_read = read(fd, buff + idx, 1);
	if (bytes_read > 0)
	{
		idx++;
		// If the first byte is ESC (Escape key), check for escape sequences
		if (buff[0] == 0x1b)
		{
			// Escape sequence, continue reading the next characters
			while (is_input_ready(fd) && idx < 9)
			{
				bytes_read = read(fd, buff + idx, 1);
				if (bytes_read < 0)
					return (null_return(buff));
				idx++;
			}
		}
		return (buff);
	}
	return (null_return(buff));
}
