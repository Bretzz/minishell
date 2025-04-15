/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checks.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 16:50:57 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/15 17:28:20 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

char	colon_pipe_check(t_token *colonipe, t_token *prev);
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

char	colon_pipe_check(t_token *colonipe, t_token *prev)
{
	if (colonipe == NULL)
		return (1);
	if (colonipe->type != TOKEN_PIPE
		&& colonipe->type != TOKEN_SEMICOL
		&& colonipe->type != TOKEN_AND_OP
		&& colonipe->type != TOKEN_OR_OP)
		return (0);
	if (prev == NULL)
		return (*colonipe->value);
	if (colonipe->next
		&& (colonipe->next->type == TOKEN_PIPE
			|| colonipe->next->type == TOKEN_SEMICOL
			|| colonipe->next->type == TOKEN_AND_OP
			|| colonipe->next->type == TOKEN_OR_OP))
		return (*colonipe->value);
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
