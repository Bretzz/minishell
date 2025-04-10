/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: march <march@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 15:45:53 by mapascal          #+#    #+#             */
/*   Updated: 2025/04/11 00:11:19 by march            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

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

t_token_type	pipe_or_die(const char *line, int *i)
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

/* ! ! ! UNUSED ! ! ! */
static char	skip_quotes(char *str, int *i, char quote)
{
	if (str[*i] == '"' || str[*i] == '\'')
	{
		if (quote == 0)
		{
			quote = str[*i];
			(*i)++;
			quote = skip_quotes(str, i, quote);
		}
		else if (quote == str[*i])
		{
			quote = 0;
			(*i)++;
			quote = skip_quotes(str, i, quote);
		}
	}
	return (quote);
}

/* str is a dynamic allocated memory addres, it will be free'd later */
/* ! ! ! UNUSED ! ! ! */
char	*remove_quotes(char *str)
{
	char	quote;
	int		j;
	int		i;
	char	*newstr;

	j = 0;
	i = 0;
	quote = 0;
	newstr = ft_calloc(ft_strlen(str) + 1, sizeof(char));
	if (!newstr)
		return (NULL);
	while (str[i])
	{
		//ft_printf("'%c'...", str[i]);
		quote = skip_quotes(str, &i, quote);
		if (str[i] == '\0')
			break ;
		newstr[j++] = str[i++];
	}
	newstr[j] = '\0';
	return (newstr);
}


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
			(*i)++;
		}
		else
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

/* static  */int	add_token(t_token **tokens, t_token_type type, char *value)
{
	t_token	*new;
	t_token	*tmp;

	if (!tokens || (type == TOKEN_WORD && !value))
		return (0);
	new = malloc(sizeof(t_token));
	if (new == NULL)
	{
		write(STDERR_FILENO, "minishell: malloc failure\n", 15);
		return (0);
	}
	new->type = type;
	if (value == NULL)
		new->value = NULL;
	else
		new->value = value
	new->next = NULL;
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

char *get_rekd(t_token_type type)
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

	if (line == NULL)
		return (NULL);
	i = 0;
	tokens = NULL;
	while (line[i])
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
