/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bush_pioneers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 23:08:31 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/15 23:12:01 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"
#include "core_bonus.h"

t_bush	*first_without_brakets(t_bush *curr_bush, int brakets);
t_logic	brakets_out(t_bush *curr_bush, int brakets);

t_bush	*first_without_brakets(t_bush *curr_bush, int brakets)
{
	curr_bush = curr_bush->next;
	while (curr_bush)
	{
		brakets += chrnum(curr_bush->sub_line, '(');
		brakets -= chrnum(curr_bush->sub_line, ')');
		curr_bush = curr_bush->next;
		if (brakets == 0)
			break ;
	}
	return (curr_bush);
}

t_logic	brakets_out(t_bush *curr_bush, int brakets)
{
	curr_bush = curr_bush->next;
	while (curr_bush)
	{
		brakets += chrnum(curr_bush->sub_line, '(');
		brakets -= chrnum(curr_bush->sub_line, ')');
		if (brakets == 0)
			break ;
		curr_bush = curr_bush->next;
	}
	if (curr_bush)
		return (curr_bush->logic);
	return (LOGIC_OR_OP);
}
