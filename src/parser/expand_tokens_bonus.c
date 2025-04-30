/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tokens_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 17:09:35 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/30 20:31:40 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"
#include "parser.h"

int	expand_tokens(t_token *tokens, const char ***vars);

static int	value_insertion(t_token *tokens, size_t flag, char *value)
{
	t_token	*new;

	if (value == NULL)
	{
		write(STDERR_FILENO, "minishell: malloc failure (dripping)\n", 37);
		return (0);
	}
	if (flag == 0)
		tokens->value = value;
	else
	{
		while (tokens->next && tokens->next->type == TOKEN_WORD)
			tokens = tokens->next;
		new = ft_calloc(1, sizeof(t_token));
		if (new == NULL)
		{
			write(STDERR_FILENO, "minishell: malloc failure\n", 26);
			return (0);
		}
		new->type = TOKEN_WORD;
		new->value = value;
		new->next = tokens->next;
		tokens->next = new;
	}
	return (1);
}

static size_t	next_real_space(char *my_str, size_t start,
	size_t c1, char quote)
{
	size_t	i;

	if (my_str == NULL)
		return (0);
	if (start == ft_strlen(my_str))
	{
		return (0);
	}
	i = start;
	while (my_str[i] != '\0')
	{
		if (c1 && quote == 0 && my_str[i] == ' ')
			return (i);
		if (quote == 0 && (my_str[i] == '"' || my_str[i] == '\''))
			quote = my_str[i];
		else if (my_str[i] == quote)
			quote = 0;
		i++;
	}
	return (0);
}

static int	loop_s_body(char **my_str, size_t *cuts,
	t_token *tokens, const char ***vars)
{
	static char		quote;
	static size_t	i;

	quote = skip_quotes((*my_str), i, quote);
	if ((*my_str)[i] == '\0' && cuts[1] == 0)
	{
		i = 0;
		return (0);
	}
	if (cuts[1] == 0 && quote != '\''
		&& ((*my_str)[i] == '$' || (*my_str)[i] == '~' || (*my_str)[i] == '*'))
		*my_str = single_expand_bonus(&i, &cuts[1], (*my_str), vars);
	else if (cuts[1] == 0)
		i++;
	cuts[1] = next_real_space((*my_str), cuts[0], cuts[1], quote);
	if (cuts[1] != 0)
	{
		if (!value_insertion(tokens, cuts[0],
				ft_substr((*my_str), cuts[0], cuts[1] - cuts[0])))
			return (free((*my_str)), 0);
		cuts[0] = cuts[1] + 1;
	}
	return (1);
}

static int	tokens_expand_string(t_token *tokens, char *str, const char ***vars)
{
	char		*my_str;
	size_t		cuts[2];

	my_str = ft_strdup(str);
	if (my_str == NULL)
		return (0);
	ft_bzero(&cuts, 2 * sizeof(size_t));
	while (loop_s_body(&my_str, cuts, tokens, vars))
		;
	if (!value_insertion(tokens, cuts[0],
			ft_substr(my_str, cuts[0], ft_strlen(&my_str[cuts[0]]))))
		return (free(my_str), 0);
	return (free(my_str), 1);
}

/* Takes a token list and the vars as parameter.
Expands every TOKEN_WORD's value with env or shell variables inside,
then removes quotes and adds new tokens if spaces are found.
RETURNS: 0 on malloc failure, 1 on successful expansion. */
int	expand_tokens(t_token *tokens, const char ***vars)
{
	char	*exp_value;
	t_token	*prev;

	(void)exp_value;
	prev = NULL;
	while (tokens)
	{
		if (tokens->type == TOKEN_WORD
			&& (!prev || prev->type != TOKEN_HERE_DOC))
		{
			tokens->not_expanded = tokens->value;
			if (!tokens_expand_string(tokens, tokens->value, vars))
			{
				write(STDERR_FILENO, "minishell: malloc failure\n", 26);
				return (0);
			}
			prev = tokens;
			tokens = tokens->next;
		}
	}
	return (1);
}
