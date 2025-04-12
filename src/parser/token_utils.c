/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 20:16:05 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/12 21:50:10 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

t_token_type	get_last_type(t_token *tokens);
t_token_type	get_next_operator(const char *line, int *i);
int				is_operator(const char *line, int i);
void			skip_spaces(const char *line, int *i);

void	skip_spaces(const char *line, int *i)
{
	while (line[*i] == ' ' || line[*i] == '\t')
		(*i)++;
}

int	is_operator(const char *line, int i)
{
	if (line[i] == '|' || line[i] == ';' || line[i] == '<' || line[i] == '>' )
		return (1);
	return (0);
}

t_token_type	get_last_type(t_token *tokens)
{
	if (tokens == NULL)
		return (TOKEN_WORD);
	while (tokens->next)
		tokens = tokens->next;
	return (tokens->type);
}

static t_token_type	pipe_or_die(const char *line, int *i)
{
	if (line[*i] == '|')
	{
		(*i)++;
		return (TOKEN_PIPE);
	}
	if (line[*i] == ';')
	{
		(*i)++;
		return (TOKEN_SEMICOL);
	}
	return (TOKEN_WORD);
}

t_token_type	get_next_operator(const char *line, int *i)
{
	if (line[*i] == '|' || line[*i] == ';')
	{
		return (pipe_or_die(line, i));
	}
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
