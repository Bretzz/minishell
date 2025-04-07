/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 23:29:52 by totommi           #+#    #+#             */
/*   Updated: 2025/04/07 14:51:50 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

int	syntax_tokens(t_token *tokens);

static char	pipe_check(t_token *pipe, t_token *prev)
{
	if (pipe == NULL)
		return (1);
	if (pipe->type != TOKEN_PIPE)
		return (0);
	if (prev == NULL)
		return (1);
	if (pipe->next && pipe->next->type == TOKEN_PIPE)
		return (1);
	return (0);
}

static char	redir_check(t_token *redir)
{
	if (redir == NULL)
		return (1);
	if (redir->type != TOKEN_RED_INPUT
		&& redir->type != TOKEN_RED_OUTPUT
		&& redir->type != TOKEN_APPEND
		&& redir->type != TOKEN_HERE_DOC)
		return (0);
	if (redir->next == NULL)
		return (1);
	if (redir->next->type != TOKEN_WORD)
		return (1);
	return (0);
}

static char	word_check(t_token *word, t_token *prev)
{
	size_t	i;
	char	file[] = "`&*()\\";
	char	command[] = "`%^&*()[]{}\\:";
	char	redir[] = "<|>";
	char	*ret;
	
	if (word == NULL)
		return (1);
	if (word->type != TOKEN_WORD)
		return (0);
	ret = NULL;
	i = 0;
	while (word->value[i] != '\0')
	{
		if (prev == NULL || prev->type == TOKEN_PIPE)
		{
			ret = ft_strchr(command, word->value[i]);
			if (ret != NULL)
				return (*ret);
		}
		else if (prev->type == TOKEN_RED_INPUT
				|| prev->type == TOKEN_RED_OUTPUT
				|| prev->type == TOKEN_APPEND)
		{
			ret = ft_strchr(file, word->value[i]);
			if (ret != NULL)
				return (*ret);
			ret = ft_strchr(redir, word->value[i]);
			if (ret != NULL)
				return (*ret);
		}
		i++;
	}
	return (0);
}

static char	is_error(char *err_code)
{
	int	i;

	i = 0;
	while (i < 3)
	{
		if (err_code[i] != 0)
			return (err_code[i]);
		i++;
	}
	return (0);
}

/* minishell syntax errors: *&` */
int	syntax_tokens(t_token *tokens)
{
	t_token	*prev;
	char	err_code[3];

	if (tokens == NULL)
		return (0);
	ft_memset(err_code, 0, 3 * sizeof(char));
	prev = NULL;
	while(tokens)
	{
		err_code[0] = redir_check(tokens);
		err_code[1] = pipe_check(tokens, prev);
		err_code[2] = word_check(tokens, prev);
		if (is_error(err_code))
			break ;
		prev = tokens;
		tokens = tokens->next;
	}
	if (tokens != NULL)
	{
		if (err_code[0] != 0)
		{
			if (tokens->next)
				ft_printfd(STDERR_FILENO, "minishell: syntax error near unexpected token `%s'\n", tokens->next->value);
			else
				ft_printfd(STDERR_FILENO, "minishell: syntax error near unexpected token `newline'\n");
			return (0);
		}
		err_code[0] = is_error(err_code);
		if (err_code[0] == 1)
			ft_printfd(STDERR_FILENO, "minishell: syntax error near unexpected token `%s'\n", tokens->value);
		else
			ft_printfd(STDERR_FILENO, "minishell: syntax error near unexpected token `%c'\n", err_code[0]);
		return (0);
	}
	return (1);
}
