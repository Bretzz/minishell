/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 19:49:12 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/06 21:21:35 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

int	syntax_check(char *line);

static void	skip_quotes(char *line, size_t *i)
{
	char	quote;

	if (line == NULL)
		return ;
	if (line[*i] == '\'' || line[*i] == '"')
	{
		quote = line[*i];
		(*i)++;
		while (line[*i] && line[*i] != quote)
			(*i)++;
		if (line[*i] == quote)
			(*i)++;
	}
}

static char	next_not_whitespace(char *line)
{
	if (line == NULL)
		return ('\0');
	line++;
	while (*line && ft_isspace(*line))
		line++;
	return (*line);
}

static int	pipe_check(char *line, char prev)
{
	char next;

	if (line == NULL)
		return (1);
	if (*line != '|')
		return (0);
	if (prev == '\0')
		return (1);
	next = next_not_whitespace(line);
	if (next == '|')
		return (1);
	return (0);
}

static int	redir_in_check(char *line)
{
	char next;
	int		is_double;

	is_double = 0;
	if (line == NULL)
		return (1);
	if (*line != '<')
		return (0);
	if (*(line + 1) == '<')
	{
		is_double = 1;
		line++;
	}
	if (*(line) == '\0')
		return (1 + is_double);
	next = next_not_whitespace(line);
	if (ft_strchr("~`!@#%^&*()[]{}+-\\/:<|>", next))
		return (1 + is_double);
	while (*(line) && ft_isspace(*line))
		line++;
	if (*(line) == '\0')
		return (1 + is_double);
	return (0);
}

static int	redir_out_check(char *line)
{
	char	next;
	int		is_double;

	is_double = 0;
	if (line == NULL)
		return (1);
	if (*line != '>')
		return (0);
	if (*(line + 1) == '>')
	{
		is_double = 1;
		line++;
	}
	if (*(line) == '\0')
		return (1 + is_double);
	next = next_not_whitespace(line);
	if (ft_strchr("~`!@#%^&*()[]{}+-\\/:<|>", next))
		return (1 + is_double);
	while (*(line) && ft_isspace(*line))
		line++;
	if (*(line) == '\0')
		return (1 + is_double);
	return (0);
}

static int	valid_char_check(char c)
{
	if (ft_strchr("~`!@#%^&*()[]{}+-\\/:", c))
		return (1);
	return (0);
}

static int	is_error(int *err_code)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		if (err_code[i] != 0)
			return (err_code[i]);
		i++;
	}
	return (0);
}

/* RETURNS: 1 if line is syntactically correct, 0 if it isn't
(writing syntax error STDERR_FILENO). */
int	syntax_check(char *line)
{
	size_t	i;
	char	next;
	char	prev;
	int		err_code[4];

	if (line == NULL)
		return (0);
	ft_memset(err_code, 0, 4 * sizeof(int));
	prev = '\0';
	i = 0;
	while (line[i] != '\0')
	{
		skip_quotes(line, &i);
		err_code[0] = valid_char_check(line[i]);
		err_code[1] = redir_in_check(&line[i]);
		err_code[2] = redir_out_check(&line[i]);
		err_code[3] = pipe_check(&line[i], prev);
		if (is_error(err_code))
		break ;
		if (!ft_isspace(line[i]))
			prev = line[i];
		i++;
	}
	if (line[i] != '\0')
	{
		if (err_code[3] != 0)
		{
			ft_printfd(STDERR_FILENO, "minishell: syntax error near unexpected token `%c'\n", line[i]);
			return (0);
		}
		err_code[0] = is_error(err_code);
		next = next_not_whitespace(&line[(i - 1) + err_code[0]]);
		if (next == '\0')
			ft_printfd(STDERR_FILENO, "minishell: syntax error near unexpected token `newline'\n");
		else
			ft_printfd(STDERR_FILENO, "minishell: syntax error near unexpected token `%c'\n", next);
		return (0);
	}
	return (1);
}
