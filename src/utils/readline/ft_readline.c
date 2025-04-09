/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_readline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 16:21:20 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/09 15:12:37 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_readline.h"
#include "minishell.h"

char	*ft_readline(const char *prompt);

/* duplicates a matrix of size 'size'.
if only 'n' < size pointer are present in the matrix
just strdups those. */
static char	**ft_mtxdup(char **mtx, size_t size)
{
	char			**new_mtx;
	unsigned int	i;

	if (mtx == NULL)
		return (NULL);
	new_mtx = ft_calloc(size, sizeof(char *));
	if (new_mtx == NULL)
		return (NULL);
	i = 0;
	while (i < size)
	{
		new_mtx[i] = ft_strdup(mtx[i]);
		if (mtx[i] && !new_mtx[i])
		{
			free_mtx((void **)new_mtx);
			return (NULL);
		}
		i++;
	}
	return (new_mtx);
}

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

/* Returns the input typed by the user,
special characters are processed instantly */
char	*ft_readline(const char *prompt)
{
	char		*line;
	char		**curr_hist;

	curr_hist = ft_mtxdup(ft_add_history(NULL), MAX_HISTORY);
	write(STDOUT_FILENO, prompt, ft_strlen(prompt));
	line = build_line(STDIN_FILENO, curr_hist);
	while (line && line[0] == '\0')
	{
		/* free(line);  */free_mtx((void **)curr_hist);	//may need to skip the first NULL
		curr_hist = ft_mtxdup(ft_add_history(NULL), MAX_HISTORY);
		write(STDOUT_FILENO, prompt, ft_strlen(prompt));
		line = build_line(STDIN_FILENO, curr_hist);
	}
	free_mtx((void **)curr_hist);	//same here
	return (line);
}
