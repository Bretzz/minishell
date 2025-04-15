/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_line.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 20:06:24 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/15 17:33:28 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "core.h"

void	handle_line(char **line, char ***vars);

/* adds the passed line to the history, then alloc(3)s
a new string without comments (#), free(3)s the originl
string and returns the new string. */
static char	*history_is_set(char *line)
{
	char	*strip_line;

	if (line == NULL)
		return (NULL);
	add_history(line);
	strip_line = drop_comment(line);
	free(line);
	return (strip_line);
}

void	handle_line(char **line, char ***vars)
{
	int	exit_code;

	if (*line == NULL)
	{
		if (*((unsigned char *)vars[0] + 6) == 1)
			write(STDOUT_FILENO, "exit\n", 5);
		clean_exit(NULL, NULL, vars, *((unsigned char *)vars[0] + 5));
	}
	exit_code = syntax_line(line, vars);
	if (exit_code == 2)
		bongou_stray_docs(*line, (const char ***)vars);
	else if (exit_code == 1)
		clean_exit(NULL, *line, vars, EXIT_FAILURE);
	*line = history_is_set(*line);
	if (*line != NULL && exit_code == 0 && !unbox_the_line(*line, vars))
		clean_exit(NULL, *line, vars, EXIT_FAILURE);
}
