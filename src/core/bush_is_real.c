/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bush_is_real.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 15:05:11 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/16 11:27:52 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"
#include "core_bonus.h"

// cc src/core/bonus_main.c libft/libft.a -I. -Ilibft -Isrc/vars/mtxd

void	print_bush(t_bush *bush)
{
	while (bush)
	{
		if (bush->true)
			ft_printf("\t/ %s\n", bush->true->sub_line);
		else
			ft_printf("\t/ KILL\n");
		ft_printf("[%s]\n", bush->sub_line);
		if (bush->false)
			ft_printf("\t\\ %s\n", bush->false->sub_line);
		else
			ft_printf("\t\\ KILL\n");
		bush = bush->next;
	}
}

static void	replace_all(char *str, int c, int r)
{
	size_t	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == c)
			str[i] = r;
		i++;
	}
}

static void	polish_bush(t_bush *bush)
{
	while (bush)
	{
		replace_all(bush->sub_line, '(', ' ');
		replace_all(bush->sub_line, ')', ' ');
		bush = bush->next;
	}
}

static void	true_or_false(t_bush *bush, int brakets)
{
	if (brakets != 0)
	{
		if (bush->logic == LOGIC_AND_OP)
		{
			bush->true = bush->next;
			if (brakets_out(bush, brakets) == LOGIC_AND_OP)
				bush->false = first_without_brakets(bush, brakets);
		}
		else if (bush->logic == LOGIC_OR_OP)
		{
			bush->false = bush->next;
			if (brakets_out(bush, brakets) == LOGIC_AND_OP)
				bush->true = first_without_brakets(bush, brakets);
		}
	}
	else
	{
		if (bush->logic == LOGIC_AND_OP)
			bush->true = bush->next;
		else if (bush->logic == LOGIC_OR_OP)
			bush->false = bush->next;
	}
}

// echo 1 && (echo 2 || echo 3) || echo 4 || (echo 5 && echo 6) (1,2)
// echo 1 && (echo2 || echo3) || echo 4 || (echo 5 && echo 6) (1, 4)
// echo 1 && (echo2 || echo3) || echo 4 && (echo 5 || echo 6) (1, 4, 5)

t_bush	*line_to_bush(char *line)
{
	t_bush	*bush[2];
	int		brakets;

	brakets = 0;
	ft_memset(bush, 0, 2 * sizeof(void *));
	bush[0] = fill_bush(line);
	bush[0]->line = line;
	if (bush[0] == NULL)
		return (NULL);
	bush[1] = bush[0];
	while (bush[1])
	{
		brakets += chrnum(bush[1]->sub_line, '(');
		brakets -= chrnum(bush[1]->sub_line, ')');
		true_or_false(bush[1], brakets);
		bush[1] = bush[1]->next;
	}
	polish_bush(bush[0]);
	return (bush[0]);
}
