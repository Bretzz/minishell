/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 14:44:34 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/02 15:31:22 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	here_doc(char *limiter);

/* Takes an string as parameters.
Fills a pipe with input from STDIN_FILENO until 'limiter' is found.
After that closes the write-end of the pipe and returns the read-end. */
int	here_doc(char *limiter)
{
	int		pipefd[2];
	char	*line;

	if (pipe(pipefd) < 0)
		return (-1);
	write(STDOUT_FILENO, ">", 1);
	line = get_next_line(STDIN_FILENO);
	while (/* line != NULL &&  */!(line && !ft_strncmp(limiter, line, ft_strlen(limiter))
			&& line[ft_strlen(limiter)] == '\n'))
	{
		if (line != NULL)
		{
			write(pipefd[1], line, ft_strlen(line));
			free(line);
		}
		else
			ft_printf("\nminishell: is weak...\n");
		write(STDOUT_FILENO, ">", 1);
		line = get_next_line(STDIN_FILENO);
	}
	close(pipefd[1]);
	free(line);
	return (pipefd[0]);
}
