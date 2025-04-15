/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stray_docs_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 21:20:11 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/15 23:54:02 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"
#include "parser_bonus.h"

/* somehow this function set's tokens' content tu NULL */
static t_token	*error_fill(void *err_code2,
	t_token *tokens, t_token *prev,
	int brakets)
{
	int	*err_code;

	err_code = (int *)err_code2;
	err_code[0] = redir_check(tokens);
	err_code[1] = colon_pipe_check(tokens, prev);
	err_code[2] = braket_check(tokens, prev, brakets);
	err_code[3] = word_check(tokens, prev);
	return (tokens);
}

static unsigned int	token_break_point(t_token *tokens)
{
	unsigned int	break_point;
	t_token			*prev;
	char			err_code[4];
	int				brakets;

	if (tokens == NULL)
		return (0);
	break_point = 0;
	brakets = 0;
	ft_memset(err_code, 0, 4 * sizeof(char));
	prev = NULL;
	while (tokens)
	{
		if (tokens->type == TOKEN_OPEN_BR)
			brakets++;
		tokens = error_fill((int *)err_code, tokens, prev, brakets);
		if (is_error_bonus(err_code))
			break ;
		if (tokens->type == TOKEN_CLOSE_BR && brakets)
			brakets--;
		prev = tokens;
		tokens = tokens->next;
		break_point++;
	}
	return (break_point);
}

static unsigned int	find_break_point(char *line)
{
	unsigned int	break_point;
	t_token			*tokens;

	tokens = tokenizer_bonus(line);
	if (tokens == NULL)
		return (0);
	break_point = token_break_point(tokens);
	free_tokens(tokens);
	return (break_point);
}

static void	open_close_doc(t_token *tokens,
	unsigned int break_point,
	const char ***vars)
{
	unsigned int	i;

	i = 0;
	while (tokens && i < break_point)
	{
		if (tokens->type == TOKEN_HERE_DOC)
		{
			tokens = tokens->next;
			if (tokens == NULL)
				break ;
			safeclose(here_doc(tokens->value, vars));
			i++;
		}
		tokens = tokens->next;
		i++;
	}
}

/* this funciton should let the user fill the docs even after a syntax error */
void	bongou_stray_docs_bonus(char *line, const char ***vars)
{
	unsigned int	break_point;
	char			*check_this;
	t_token			*tokens;

	if (line == NULL || is_white(line))
		return ;
	check_this = drop_comment(line);
	if (check_this == NULL)
		return ;
	break_point = find_break_point(check_this);
	tokens = tokenizer_bonus(check_this);
	free(check_this);
	if (tokens == NULL)
		return ;
	open_close_doc(tokens, break_point, vars);
	free_tokens(tokens);
}
