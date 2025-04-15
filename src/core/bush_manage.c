/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bush_manage.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 23:02:36 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/15 23:24:01 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"
#include "core_bonus.h"

t_bush	*add_new(t_bush **head, char *sub_line, t_logic logic);
t_bush	*fill_bush(char *line);
void	free_bush(t_bush *bush);

void	free_bush(t_bush *bush)
{
	t_bush	*tmp;

	while (bush)
	{
		tmp = bush;
		bush = bush->next;
		free(tmp);
	}
}

static int	is_and(char *str)
{
	if (str == NULL)
		return (0);
	if (str[0] == '&' && str[1] == '&')
		return (1);
	return (0);
}

static int	is_or(char *str)
{
	if (str == NULL)
		return (0);
	if (str[0] == '|' && str[1] == '|')
		return (1);
	return (0);
}

t_bush	*add_new(t_bush **head, char *sub_line, t_logic logic)
{
	t_bush	*node;
	t_bush	*tmp;

	node = ft_calloc(1, sizeof(t_bush));
	if (node == NULL || head == NULL)
		return (free(node), NULL);
	node->sub_line = sub_line;
	node->logic = logic;
	node->next = NULL;
	if (*head == NULL)
	{
		*head = node;
		return (*head);
	}
	tmp = *head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = node;
	return (*head);
}

t_bush	*fill_bush(char *line)
{
	size_t		last_crop;
	size_t		i;
	t_bush		*bush;
	t_logic		logic;

	bush = NULL;
	last_crop = 0;
	i = 0;
	while (line[i] != '\0')
	{
		if (is_and(&line[i]) || is_or(&line[i]))
		{
			if (line[i] == '&')
				logic = LOGIC_AND_OP;
			else
				logic = LOGIC_OR_OP;
			if (!add_new(&bush, &line[last_crop], logic))
				return (free_bush(bush), NULL);
			line[i++] = '\0';
			line[i] = '\0';
			last_crop = i + 1;
		}
		i++;
	}
	return (add_new(&bush, &line[last_crop], LOGIC_AND_OP), bush);
}
