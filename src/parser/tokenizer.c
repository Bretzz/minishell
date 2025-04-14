/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapascal <mapascal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 15:45:53 by mapascal          #+#    #+#             */
/*   Updated: 2025/04/14 11:33:48 by mapascal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

/* returns a dynamicly allocated mem addres */
static char	*get_next_word(const char *line, int *i)
{
	int		start;
	char	quote;
	char	*word;

	start = *i;
	quote = 0;
	word = NULL;
	while (line[*i] && (quote || (line[*i] != ' ' && !is_operator(line, *i))))
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
	{
		write(STDERR_FILENO, "minishell: malloc failure\n", 26);
		return (NULL);
	}
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
		write(STDERR_FILENO, "minishell: malloc failure\n", 15);
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

static char	*get_rekd(t_token_type type)
{
	if (type == TOKEN_PIPE)
		return (ft_strdup("|"));
	else if (type == TOKEN_SEMICOL)
		return (ft_strdup(";"));
	else if (type == TOKEN_RED_INPUT)
		return (ft_strdup("<"));
	else if (type == TOKEN_RED_OUTPUT)
		return (ft_strdup(">"));
	else if (type == TOKEN_APPEND)
		return (ft_strdup(">>"));
	else if (type == TOKEN_HERE_DOC)
		return (ft_strdup("<<"));
	return (NULL);
}

t_token	*tokenizer(char *line)
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
		if (is_operator(line, i))
		{
			type = get_next_operator(line, &i);
			if (!add_token(&tokens, type, get_rekd(type)))
				return (free_tokens(tokens), NULL);
		}
		else
		{
			if (!add_token(&tokens, TOKEN_WORD, get_next_word(line, &i)))
				return (free_tokens(tokens), NULL);
		}
	}
	return (tokens);
}

void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		free(tmp->value);
		free(tmp->not_expanded);
		free(tmp);
	}
}

void	print_tokens(t_token *tokens)
{
	while (tokens)
	{
		ft_printf("Token: ");
		if (tokens->type == TOKEN_WORD)
			ft_printf("WORD (%s)", tokens->value);
		else if (tokens->type == TOKEN_PIPE)
			ft_printf("PIPE (|)");
		else if (tokens->type == TOKEN_RED_INPUT)
			ft_printf("REDIRECT_IN (<)");
		else if (tokens->type == TOKEN_RED_OUTPUT)
			ft_printf("REDIRECT_OUT (>)");
		else if (tokens->type == TOKEN_HERE_DOC)
			ft_printf("HERE_DOC (<<)");
		else if (tokens->type == TOKEN_APPEND)
			ft_printf("APPEND (>>)");
		else if (tokens->type == TOKEN_SEMICOL)
			ft_printf("SEMICOLON (;)");
		else if (tokens->type == TOKEN_S_QUOTE)
			ft_printf("SINGLE QUOTE (')");
		else if (tokens->type == TOKEN_D_QUOTE)
			ft_printf("DOUBLE QUOTE (\")");
		ft_printf("\n");
		tokens = tokens->next;
	}
}
