/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 20:16:05 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/15 23:35:02 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"
#include "parser_bonus.h"

t_token_type	get_next_operator_bonus(const char *line, int *i);
int				is_operator_bonus(const char *line, int i);

int	is_operator_bonus(const char *line, int i)
{
	if (line[i] == '|'
		|| line[i] == ';'
		|| line[i] == '<'
		|| line[i] == '>'
		|| line[i] == '&'
		|| line[i] == '('
		|| line[i] == ')')
		return (1);
	return (0);
}

static t_token_type	brakets_or_die(const char *line, int *i)
{
	ft_printf("in brakets or die '%s'\n", line);
	if (line[*i] == '&' && line[*i + 1] == '&')
	{
		(*i) += 2;
		return (TOKEN_OR_OP);
	}
	else if (line[*i] == '(')
	{
		(*i)++;
		return (TOKEN_OPEN_BR);
	}
	else if (line[*i] == ')')
	{
		(*i)++;
		return (TOKEN_CLOSE_BR);
	}
	return (TOKEN_WORD);
}

static t_token_type	pipe_or_die(const char *line, int *i)
{
	ft_printf("in pipe_or_die '%s'\n", line);
	if (line[*i] == ';')
	{
		(*i)++;
		return (TOKEN_SEMICOL);
	}
	else if (line[*i] == '|')
	{
		if (line[*i + 1] == '|')
		{
			*i += 2;
			return (TOKEN_OR_OP);
		}
		(*i)++;
		return (TOKEN_PIPE);
	}
	return (TOKEN_WORD);
}

t_token_type	get_next_operator_bonus(const char *line, int *i)
{
	if (line[*i] == '|' || line[*i] == ';')
		return (pipe_or_die(line, i));
	if (line[*i] == '&' || line[*i] == '(' || line[*i] == ')')
		return (brakets_or_die(line, i));
	if (line[*i] == '<')
	{
		if (line[*i + 1] == '<')
		{
			*i += 2;
			return (TOKEN_HERE_DOC);
		}
		(*i)++;
		return (TOKEN_RED_INPUT);
	}
	if (line[*i] == '>')
	{
		if (line[*i + 1] == '>')
		{
			*i += 2;
			return (TOKEN_APPEND);
		}
		(*i)++;
		return (TOKEN_RED_OUTPUT);
	}
	return (TOKEN_WORD);
}
