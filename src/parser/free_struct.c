/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_struct.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 22:05:46 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/15 22:07:21 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		free(tmp->value);
		free(tmp->not_expanded);
		free(tmp);
	}
}

/* frees the cmd array */
void	free_cmd(t_cmd *cmd_arr)
{
	int	i[2];

	if (cmd_arr == NULL)
		return ;
	i[0] = 0;
	while (cmd_arr[i[0]].words[0])
	{
		i[1] = 0;
		while (cmd_arr[i[0]].words[i[1]])
			free(cmd_arr[i[0]].words[i[1]++]);
		i[0]++;
	}
	free(cmd_arr);
}
