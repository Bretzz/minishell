/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getchar.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 14:23:01 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/09 13:23:27 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_readline.h"
#include "libft.h"

char	*ft_getchar(int fd);

/* This functions read(3) one byte at the time from 'fd'
until there are no more bytes to read (is_input_ready()).
RETURNS: 1 on successful execution, 0 on read failure (bad fd, SIGINT). */
static int	read_whole_input(int fd, char *buff)
{
	ssize_t				bytes_read;
	unsigned int		idx;

	idx = 0;
	bytes_read = read(fd, buff + idx, 1);
	if (bytes_read < 0)
		return (0);
	idx++;
	// If the first byte is ESC (Escape key), check for escape sequences
	if (buff[0] == 0x1b)
	{
		// Escape sequence, continue reading the next characters
		while (is_input_ready(fd) && idx < 9)
		{
			bytes_read = read(fd, buff + idx, 1);
			if (bytes_read < 0)
				return (0);
			idx++;
		}
	}
	return (1);
}

/* Reads a single char from fd, then returns the buffer.
NOTE: if an escape sequence is sent, further read(3) calls are
made until the whole sequence is stored in buff. */
char *ft_getchar(int fd)
{
	char	*buff;

	buff = (char *)ft_calloc(10, sizeof(char));
	if (buff == NULL)
	{
		write(STDERR_FILENO, "ft_getchar: malloc failure\n", 27);
		return (NULL);
	}
	if (read_whole_input(fd, buff) != 0)
		return (buff);
	free(buff);
	return (NULL);
}
