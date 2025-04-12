/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 20:16:05 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/12 20:16:27 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

t_token_type get_last_type(t_token *tokens);

t_token_type get_last_type(t_token *tokens)
{
	if (tokens == NULL)
		return (TOKEN_WORD);
	while (tokens->next)
		tokens = tokens->next;
	return (tokens->type);
}
