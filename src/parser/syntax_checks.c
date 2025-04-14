/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checks.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapascal <mapascal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 16:50:57 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/14 12:16:13 by mapascal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

char	pipe_check(t_token *pipe, t_token *prev);
char	redir_check(t_token *redir);
char	word_check(t_token *word, t_token *prev);
char	is_error(char *err_code);

char	is_error(char *err_code)
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

char	pipe_check(t_token *pipe, t_token *prev)
{
	if (pipe == NULL)
		return (1);
	if (pipe->type != TOKEN_PIPE)
		return (0);
	if (prev == NULL)
		return ('|');
	if (pipe->next && pipe->next->type == TOKEN_PIPE)
		return ('|');
	return (0);
}

char	redir_check(t_token *redir)
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

static char	who_is_sus_here(t_token *word, t_token *prev,
	const char **blacklist)
{
	size_t	i;
	char	*ret[2];

	ft_bzero(ret, 2 * sizeof(char *));
	i = 0;
	while (word->value[i] != '\0')
	{
		if (prev == NULL || prev->type == TOKEN_PIPE)
			ret[0] = ft_strchr(blacklist[0], word->value[i]);
		else if (prev->type == TOKEN_RED_INPUT
			|| prev->type == TOKEN_RED_OUTPUT
			|| prev->type == TOKEN_APPEND)
		{
			ret[1] = ft_strchr(blacklist[1], word->value[i]);
			ret[1] = ft_strchr(blacklist[2], word->value[i]);
		}
		if (ret[0] != NULL)
			return (*ret[0]);
		if (ret[1] != NULL)
			return (*ret[0]);
		i++;
	}
	return ('\0');
}

char	word_check(t_token *word, t_token *prev)
{
	const char	command[] = "`%^&*()[]{}\\:";
	const char	file[] = "`&*()\\";
	const char	redir[] = "<|>";
	const char	*blacklist[3];

	if (word == NULL)
		return (1);
	if (word->type != TOKEN_WORD)
		return (0);
	blacklist[0] = command;
	blacklist[1] = file;
	blacklist[2] = redir;
	return (who_is_sus_here(word, prev, blacklist));
}
