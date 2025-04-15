/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_tiny.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 22:08:59 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/15 22:09:50 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

t_token_type	get_last_type(t_token *tokens);
void			skip_spaces(const char *line, int *i);

t_token_type	get_last_type(t_token *tokens)
{
	if (tokens == NULL)
		return (TOKEN_WORD);
	while (tokens->next)
		tokens = tokens->next;
	return (tokens->type);
}

void	skip_spaces(const char *line, int *i)
{
	while (line[*i] == ' ' || line[*i] == '\t')
		(*i)++;
}
