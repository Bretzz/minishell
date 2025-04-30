/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 15:45:53 by mapascal          #+#    #+#             */
/*   Updated: 2025/04/30 15:54:25 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"
#include "parser_bonus.h"

/* returns a dynamicly allocated mem addres */
static char	*get_next_word_bonus(const char *line, int *i)
{
	int		start;
	char	quote;
	char	*word;

	start = *i;
	quote = 0;
	word = NULL;
	while (line[*i] && (quote || (line[*i] != ' '
				&& !is_operator_bonus(line, *i))))
	{
		if (line[*i] == '"' || line[*i] == '\'')
		{
			if (quote == 0)
				quote = line[*i];
			else if (quote == line[*i])
				quote = 0;
		}
		(*i)++;
	}
	word = ft_substr(line, start, *i - start);
	if (word == NULL)
		write(STDERR_FILENO, "minishell: malloc failure\n", 26);
	return (word);
}

static int	add_token(t_token **tokens, t_token_type type, char *value)
{
	t_token	*new;
	t_token	*tmp;

	if (!tokens || (type == TOKEN_WORD && !value))
		return (0);
	new = ft_calloc(1, sizeof(t_token));
	if (new == NULL)
	{
		write(STDERR_FILENO, "minishell: malloc failure\n", 26);
		return (0);
	}
	new->type = type;
	new->value = value;
	new->next = NULL;
	new->not_expanded = NULL;
	if (!(*tokens))
		*tokens = new;
	else
	{
		tmp = *tokens;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
	return (1);
}

static char	*get_rekd_bonus(t_token_type type)
{
	if (type == TOKEN_PIPE)
		return (ft_strdup("|"));
	else if (type == TOKEN_SEMICOL)
		return (ft_strdup(";"));
	else if (type == TOKEN_AND_OP)
		return (ft_strdup("&&"));
	else if (type == TOKEN_OR_OP)
		return (ft_strdup("||"));
	else if (type == TOKEN_RED_INPUT)
		return (ft_strdup("<"));
	else if (type == TOKEN_RED_OUTPUT)
		return (ft_strdup(">"));
	else if (type == TOKEN_APPEND)
		return (ft_strdup(">>"));
	else if (type == TOKEN_HERE_DOC)
		return (ft_strdup("<<"));
	else if (type == TOKEN_OPEN_BR)
		return (ft_strdup("("));
	else if (type == TOKEN_CLOSE_BR)
		return (ft_strdup(")"));
	return (NULL);
}

t_token	*tokenizer_bonus(char *line)
{
	t_token			*tokens;
	t_token_type	type;
	int				i;

	i = 0;
	tokens = NULL;
	while (line && line[i])
	{
		skip_spaces(line, &i);
		if (line[i] == '\0')
			break ;
		if (is_operator_bonus(line, i))
		{
			type = get_next_operator_bonus(line, &i);
			if (!add_token(&tokens, type, get_rekd_bonus(type)))
				return (free_tokens(tokens), NULL);
		}
		else
		{
			if (!add_token(&tokens, TOKEN_WORD, get_next_word_bonus(line, &i)))
				return (free_tokens(tokens), NULL);
		}
	}
	return (tokens);
}
