/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_readline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totommi <totommi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 16:21:20 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/08 02:13:24 by totommi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	trim_back_nl(char *str);
static char	*get_safe_line(int fd);

/* DESCRIPTION
	readline  will read a line from the terminal and return it, using prompt as
	a prompt.  If prompt is NULL or the empty string, no prompt is issued.  The
	line  returned  is  allocated  with malloc(3); the caller must free it when
	finished.  The line returned has the final newline  removed,  so  only  the
	text of the line remains.

	readline  offers  editing capabilities while the user is entering the line.
	By default, the line editing commands are similar to  those  of  emacs.   A
	vi-style line editing interface is also available.

	This  manual page describes only the most basic use of readline.  Much more
	functionality is available; see The GNU Readline Library and The  GNU  His‐
	tory Library for additional information.

RETURN VALUE
	readline returns the text of the line read.  A blank line returns the empty
	string.  If EOF is encountered while reading a line, and the line is empty,
	NULL  is  returned.  If an EOF is read with a non-empty line, it is treated
	as a newline. */

char	*ft_readline(const char *prompt)
{
	char	*line;

	write(STDOUT_FILENO, prompt, ft_strlen(prompt));
	//input_initializer();
	line = get_safe_line(STDIN_FILENO);
	if (line != NULL)
		trim_back_nl(line);
	return (line);
}

/* takes a string as a parameter.
Replace the trailing nl with a null-terminator if present.
RETURNS: 1 if the newline is found (and replaced), 0 if it isn't.
NOTE: the string will be double null-terminated. */
static int	trim_back_nl(char *str)
{
	while (*str)
		str++;
	if (*(--str) == '\n')
	{
		*str = '\0';
		return (1);
	}
	return (0);
}

/* returns a line read from the 'fd'.
If a SIGQUIT or a SIGTSTP is intercepted tries again. */
static char	*get_safe_line(int fd)
{
	char	*line;
	
	line = get_next_line(fd);
	while (!line && (g_last_sig == SIGQUIT || g_last_sig == SIGTSTP))
	{
		g_last_sig = 0;
		line = get_next_line(fd);
	}
	return (line);
}
