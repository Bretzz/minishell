/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_tiny.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapascal <mapascal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 21:28:24 by mapascal          #+#    #+#             */
/*   Updated: 2025/04/14 21:35:40 by mapascal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

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

int	cmds_count(const t_token *tokens)
{
	unsigned int	count;

	count = 0;
	while (tokens)
	{
		if (tokens->type == TOKEN_PIPE
			|| tokens->type == TOKEN_SEMICOL)
			count++;
		tokens = tokens->next;
	}
	return (count + 2);
}

void	raccattagarbage(t_cmd garbage)
{
	safeclose(garbage.fd[0]);
	safeclose(garbage.fd[1]);
}

int	ft_cmdlen(t_cmd *cmd_array)
{
	size_t	i;

	i = 0;
	if (cmd_array == NULL)
		return (0);
	while (cmd_array[i].words[0] != NULL && cmd_array[i].words[0][0] != '\0')
		i++;
	return (i);
}
