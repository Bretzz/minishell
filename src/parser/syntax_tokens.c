/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 23:29:52 by totommi           #+#    #+#             */
/*   Updated: 2025/04/15 12:36:36 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

int	syntax_tokens(t_token *tokens);

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
	err_code[0] = is_error(err_code);
	if (err_code[0] == 1)
		ft_printfd(STDERR_FILENO, "minishell: \
syntax error near unexpected end of file\n");
	else
		ft_printfd(STDERR_FILENO, "minishell: \
syntax error near unexpected token `%c'\n", err_code[0]);
	return ;
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
	while (tokens)
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
		return (error_print(err_code, tokens), 0);
	return (1);
}
