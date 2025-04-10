/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rank_up.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: march <march@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 17:46:02 by march             #+#    #+#             */
/*   Updated: 2025/04/10 17:53:12 by march            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Checks if the variable is present in exp, if it is does nothing.
Otherwise checks if it's in shv, in that case adds it to exp and env 
and removes it from shv. */
static void	rank_up(char *varstr, char ***vars)
{
	char	name[MAX_NAME];
	int		index;

	vstr_getname(varstr, name, MAX_NAME);
	index = mtx_getindex(name, vars[1]);
	if (index >= 0)
		return ;
	index = mtx_getindex(name, vars[0]);
	if (index >= 0)
	{
		vars[1] = mtx_vstr_copy(vars[0][index], vars[1]);
		mtx_safedel(index, vars[0]);
	}
	else
		vars[1] = mtx_setval(name, NULL, vars[1]);
}
