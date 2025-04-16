/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_redir_out.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 16:05:30 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/16 16:13:51 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

void	create_redir_out(t_token *tokens)
{
	int	fd;

	fd = 0;
	while (tokens)
	{
		if (tokens->type == TOKEN_RED_OUTPUT && tokens->next)
		{
			fd = open(tokens->next->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			tokens = tokens->next;
		}
		else if (tokens->type == TOKEN_APPEND && tokens->next)
		{
			fd = open(tokens->next->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
			tokens = tokens->next;
		}
		safeclose(fd);
		if (fd < 0)
			return ;
		tokens = tokens->next;
	}
}
