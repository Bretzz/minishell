/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_line.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapascal <mapascal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 11:32:32 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/14 12:20:34 by mapascal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

int		syntax_line(char **line, char ***vars);
char	*drop_comment(char *line);

static void	clean_exit(char *line, char *check_this, t_token *tokens)
{
	if (line != NULL)
		add_history(line);
	free(check_this);
	free_tokens(tokens);
}

static char	*safe_drop_and_free(char *line, int i)
{
	char	*drop;

	drop = drop_string(line, i, ft_strlen_nl(&line[i]));
	if (drop == NULL)
	{
		write(STDERR_FILENO, "minishell: malloc failure\n", 26);
		return (free(line), NULL);
	}
	free(line);
	return (drop);
}

/* takes a string and returns a substr from 0 to the first '#' found
not inside quotes replaced with '\0' */
char	*drop_comment(char *line)
{
	size_t	i;
	char	quote;
	char	*my_line;

	if (line == NULL)
		return (NULL);
	my_line = ft_strdup(line);
	if (my_line == NULL)
		write(STDERR_FILENO, "minishell: malloc failure\n", 26);
	quote = 0;
	i = 0;
	while (my_line && my_line[i] != '\0')
	{
		if (quote == 0
			&& (my_line[i] == '\'' || my_line[i] == '"'))
			quote = my_line[i];
		else if (my_line[i] == quote)
			quote = 0;
		if ((my_line[i] == '#') && quote == 0
			&& (i == 0 || ft_isspace(my_line[i - 1])))
			my_line = safe_drop_and_free(my_line, i);
		i++;
	}
	return (my_line);
}

/* does an syntax check on the passed line.
RETURNS: 1 on malloc failure, 1 on syntax error, 0 if all clear. */
static int	single_check(char **line, t_token_type *last_type)
{
	char	*check_this;
	t_token	*tokens;

	if (*line == NULL)
		return (0);
	check_this = drop_comment(*line);
	if (check_this == NULL)
		return (1);
	tokens = tokenizer(check_this);
	if (!syntax_tokens(tokens))
	{
		clean_exit(*line, check_this, tokens);
		return (2);
	}
	*last_type = get_last_type(tokens);
	free_tokens(tokens);
	free(check_this);
	return (0);
}

/* checks weather a line is syntattically correct and expands it if necessary.
Assign the new allocated space to the pointer pointed by *line.
RETURNS: the exit_code, 1 on malloc failure, 2 on syntax error, 0 on successful
execution.
NOTE: if a malloc failure or a syntax error occurs,
the last instance of 'line' isn't free'd, it's stored in *line. */
int	syntax_line(char **line, char ***vars)
{
	t_token_type	last_type;
	int				exit_code;

	exit_code = single_check(line, &last_type);
	if (exit_code != 0)
	{
		mtx_setdata(exit_code, vars[0], 1);
		return (exit_code);
	}
	while (!is_closed(*line))
	{
		*line = append_line(*line, last_type, vars);
		exit_code = single_check(line, &last_type);
		if (exit_code != 0)
		{
			mtx_setdata(exit_code, vars[0], 1);
			return (exit_code);
		}
	}
	return (0);
}
