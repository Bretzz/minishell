/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_tokens_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 21:51:23 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/15 23:33:31 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"
#include "parser_bonus.h"

int		syntax_tokens_bonus(t_token *tokens);
char	is_error_bonus(char *err_code);
char	braket_check(t_token *braket, t_token *prev, int is_open);

static void	error_print(char *err_code, t_token *tokens)
{
	if (err_code[0] != 0)
	{
		if (tokens->next && err_code[0] == 1)
			ft_printfd(STDERR_FILENO, "minishell: \
syntax error near unexpected token `%s'\n", tokens->next->value);
		else
			ft_printfd(STDERR_FILENO, "minishell: \
syntax error near unexpected token `newline'\n");
		return ;
	}
	if (err_code[2] == 1)
	{
		ft_printfd(STDERR_FILENO, "minishell: \
syntax error near unexpected token `newline'\n");
		return ;
	}
	err_code[0] = is_error_bonus(err_code);
	if (err_code[0] == 1)
		ft_printfd(STDERR_FILENO, "minishell: \
syntax error near unexpected end of file\n");
	else
		ft_printfd(STDERR_FILENO, "minishell: \
syntax error near unexpected token `%c'\n", err_code[0]);
	return ;
}

char	braket_check(t_token *braket, t_token *prev, int is_open)
{
	if (braket == NULL)
		return (1);
	if (braket->type != TOKEN_OPEN_BR
		&& braket->type != TOKEN_CLOSE_BR)
		return (0);
	if (braket->type == TOKEN_CLOSE_BR && !is_open)
		return (*braket->value);
	else if (braket->type == TOKEN_OPEN_BR)
	{
		if (!prev || prev->type != TOKEN_AND_OP
			|| prev->type != TOKEN_OR_OP)
			return (*braket->value);
		else if (braket->next == NULL)
			return (1);
		else if (braket->next->type == TOKEN_PIPE
			|| braket->next->type == TOKEN_SEMICOL
			|| braket->next->type == TOKEN_AND_OP
			|| braket->next->type == TOKEN_OR_OP
			|| braket->next->type == TOKEN_CLOSE_BR)
			return (*braket->next->value);
	}
	return (0);
}

char	is_error_bonus(char *err_code)
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

static void	error_fill(int *err_code,
	t_token *tokens, t_token *prev,
	int brakets)
{
	err_code[0] = redir_check(tokens);
	err_code[1] = colon_pipe_check(tokens, prev);
	err_code[2] = braket_check(tokens, prev, brakets);
	err_code[3] = word_check(tokens, prev);
}

/* minishell syntax errors: *&` */
int	syntax_tokens_bonus(t_token *tokens)
{
	t_token	*prev;
	char	err_code[4];
	int		brakets;

	if (tokens == NULL)
		return (0);
	brakets = 0;
	ft_memset(err_code, 0, 4 * sizeof(char));
	prev = NULL;
	while (tokens)
	{
		if (tokens->type == TOKEN_OPEN_BR)
			brakets++;
		error_fill((int *)err_code, tokens, prev, brakets);
		if (is_error_bonus(err_code))
			break ;
		if (tokens->type == TOKEN_CLOSE_BR && brakets)
			brakets--;
		prev = tokens;
		tokens = tokens->next;
	}
	if (tokens != NULL)
		return (error_print(err_code, tokens), 0);
	return (1);
}
