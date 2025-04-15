/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_tiny.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 21:28:24 by mapascal          #+#    #+#             */
/*   Updated: 2025/04/15 20:49:44 by topiana-         ###   ########.fr       */
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

void	raccattagarbage(t_cmd *cmd_arr)
{
	unsigned int	i;
	t_cmd			dead_cmd;

	ft_bzero(&dead_cmd, sizeof(t_cmd));
	i = 0;
	while (ft_memcmp(&cmd_arr[i], &dead_cmd, sizeof(t_cmd)))
	{
		if (cmd_arr[i].words[0] == NULL)
		{
			safeclose(cmd_arr[i].fd[0]);
			safeclose(cmd_arr[i].fd[1]);
		}
		i++;
	}
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
