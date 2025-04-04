/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totommi <totommi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 15:45:53 by mapascal          #+#    #+#             */
/*   Updated: 2025/04/05 18:27:41 by totommi          ###   ########.fr       */
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


// substr() fino a $ poi expand() fino a varlen(alfanum primo alfa, underscore) e fai strjoin(substr + exland)
// line = ciao $pippo 'sono $topolino'


// if line[i] = '$'
// explodedline = strjoin(explodedline, substr(line fino a i-1)
// if line[i] = " "


// char	*funzionebuffa(const char *line, int *i, char quote, char ***vars)
// {
	
// 	if (quote == '"')
// 		if (line[i] == '$')
			
// 	else if (quote == '\'')
// 		non esplodere le vairabili 
// }


/* returns a dynamicly allocated mem addres */
static char	*get_next_word(t_token_type prev_type, const char *line, int *i, const char ***vars)
{
	int		start;
	char	quote;
	char	*word;
	char	*cleaned_word;

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
		write(STDERR_FILENO, "malloc failure\n", 15);
		return (NULL);
	}
	// cleaned_word = remove_quotes(word);
	// free(word);
	if (prev_type != TOKEN_HERE_DOC)
		cleaned_word = expand_string(word, vars);
	else
		cleaned_word = ft_strdup(word);
	if (!cleaned_word)
		write(STDERR_FILENO, "malloc failure\n", 15);
	free(word);
	return (cleaned_word);
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
		write(STDERR_FILENO, "malloc failure\n", 15);
		return (0);
	}
	new->type = type;
	if (value == NULL)
		new->value = NULL;
	else
		new->value = value/* ft_strdup(value) */;
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

/* ! ! ! UNUSED FUNCTION ! ! ! */
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
 
static t_token_type last_token_type(t_token *tokens)
{
	if (tokens == NULL)
		return (TOKEN_WORD);
	while(tokens->next != NULL)
		tokens = tokens->next;
	return (tokens->type);
}

t_token	*tokenizer(char *line, const char ***vars)
{
	t_token		*tokens;
	int			i;

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
			if (!add_token(&tokens, get_next_operator(line, &i), NULL))
				return (free_tokens(tokens), NULL);
		}
		else
		{
			if (!add_token(&tokens, TOKEN_WORD, get_next_word(last_token_type(tokens), line, &i, vars)))
				return (free_tokens(tokens), NULL);
		}
	}
	print_tokens(tokens);
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

/* copies the enviroment passed as parameters and returns
the newly initialized matrix. */
// static char	**env_copy(char **his_env)
// {
// 	size_t	i;
// 	char **my_env;

// 	if (his_env == NULL)
// 		return (NULL);
// 	my_env = mtx_init();
// 	if (my_env == NULL)
// 		return (NULL);
// 	i = 0;
// 	while(his_env[i] != NULL)
// 	{
// 		my_env = mtx_vstr_copy(his_env[i], my_env);
// 		i++;
// 	}
// 	return (my_env);
// }

// int	main(int argc, char *argv[])
// {
// 	t_token	*tokens;
// 	char **vars[3];
// 	vars[2] = env_copy(__environ);
// 	vars[1] = NULL;
// 	vars[0] = NULL;
	
// 	(void)argc;
// 	printf("tokenizing: ===%s===\n", argv[1]);
// 	tokens = tokenizer(argv[1], (const char ***)vars);
// 	print_tokens(tokens);
// 	return (0);
// }
