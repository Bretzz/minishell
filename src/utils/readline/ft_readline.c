/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_readline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totommi <totommi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 16:21:20 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/09 00:53:42 by totommi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_readline.h"
#include "minishell.h"

//static int	trim_back_nl(char *str);
static char	*build_safe_line(int fd, char **history);
static char	*ft_strjoin_free(char *s1, char *s2);
static char	**ft_mtxdup(char **mtx, size_t size);

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

	curr_hist = ft_mtxdup(ft_get_history(), MAX_HISTORY);
	write(STDOUT_FILENO, prompt, ft_strlen(prompt));
	line = build_safe_line(STDIN_FILENO, curr_hist);
	while (line && line[0] == '\0')
	{
		/* free(line);  */free_mtx((void **)curr_hist);	//may need to skip the first NULL
		curr_hist = ft_mtxdup(ft_get_history(), MAX_HISTORY);
		write(STDOUT_FILENO, prompt, ft_strlen(prompt));
		line = build_safe_line(STDIN_FILENO, curr_hist);
	}
	free_mtx((void **)curr_hist);	//same here
	return (line);
}

static void	delete_key(int num)
{
	int	i;

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

static void	move_key(int arrow_key)
{
	if (arrow_key < 0)
		write(STDOUT_FILENO, "\b", 1);
	else
		write(STDOUT_FILENO, "\a", 1);
}

static void ft_redisplay(int *typed, char *new_line)
{
	delete_key(*typed);
	write(STDOUT_FILENO, new_line, ft_strlen(new_line));
	*typed = ft_strlen(new_line);
}

static int cycle_history(int arrow_key, char **history, int *hist_line)
{
	if (arrow_key != 0)
	{
		if (*hist_line + arrow_key < 0
			|| *hist_line + arrow_key > MAX_HISTORY
			|| history[*hist_line + arrow_key] == NULL
			|| arrow_key % 2 == 0)
			return (1);
		*hist_line += arrow_key;
		// delete_key(*typed);
		// write(STDOUT_FILENO, history[*hist_line], ft_strlen(history[*hist_line]));
		// *typed = ft_strlen(history[*hist_line]);
		return (1);
	}
	return (0);
}

/* Reads a single input from the 'fd',
if a SIGINT is recieved returns NULL,
if a SIGQUIT or a SIGTSTP another read is done. */
static char	*safe_getchar(int fd)
{
	char	*input;

	input = ft_getchar(fd);
	while (input == NULL)
	{
		if (g_last_sig == SIGINT)
			return (NULL);
		else if (g_last_sig == SIGQUIT || g_last_sig == SIGTSTP)
		{
			g_last_sig = 0;
			input = ft_getchar(fd);
		}
	}
	return (input);
}

/* Reads the input from the terminal byte by byte,
handling eventual escape sequence with handle_special_keys().
RETURNS: the input read (up to '\n'), NULL on SIGINT or malloc failure. */
static char	*build_safe_line(int fd, char **history)
{
	struct termios	old_tio;
	char			*input;
	int				hist_line;
	int				typed;

	hist_line = 0;
	typed = 0;
	set_terminal_mode(&old_tio);
	history[hist_line] = ft_strdup("");
	while (history[hist_line] != NULL)
	{
		input = safe_getchar(fd);
		if (input == NULL || is_end_of_trans(input))
		{
			free(input);
			//free(history[hist_line]);
			//history[0] = NULL;
			return (NULL);
		}
		if (is_arrow_key(input))
		{
			if (is_arrow_key(input) % 2 != 0)
			{
				cycle_history(is_arrow_key(input), history, &hist_line);
				ft_redisplay(&typed, history[hist_line]);
			}
			else
				move_key(is_arrow_key(input));
			free(input);
			continue ;
		}
		if (is_delete_key(input))
		{
			if (typed != 0)
			{
				history[hist_line][typed - 1] = '\0';
				delete_key(1);
				typed--;
			}
			free(input);
			continue ;
		}
		write(STDOUT_FILENO, input, 1);
		typed++;
		if (is_newline(input))
		{
			free(input);
			if (*history[hist_line] == '\0')
				return (history[hist_line]);
			break ;
		}
		//ft_printf("typed: %d\n", typed);
		history[hist_line] = ft_strjoin_free(history[hist_line], input);
		if (history[hist_line] == NULL)
			write(STDERR_FILENO, "ft_readline: malloc failure\n", 28);
		free(input);
		// if (history[hist_line] && ft_strchr(history[hist_line], '\n'))
		// 	break ;
	}
	restore_terminal_mode(&old_tio);
	// if (hist_line != 0)
	// {
	// 	free(history[hist_line]);
	// 	history[hist_line] = NULL;
	// 	return (ft_strdup(history[hist_line]));
	// }
	return (ft_strdup(history[hist_line]));
}

/* takes a string as a parameter.
Replace the trailing nl with a null-terminator if present.
RETURNS: 1 if the newline is found (and replaced), 0 if it isn't.
NOTE: the string will be double null-terminated. */
/* static  */int	trim_back_nl(char *str)
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

/* strjoin without stlcpy and strlcat
and frees s1 */
static char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*twelve;
	size_t	size;

	size = ft_strlen(s1) + ft_strlen(s2) + 1;
	twelve = (char *)ft_calloc(size, sizeof(char));
	if (twelve == NULL)
		return (NULL);
	ft_strlcpy(twelve, s1, size);
	ft_strlcat(twelve, s2, size);
	free(s1);
	return (twelve);
}

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
