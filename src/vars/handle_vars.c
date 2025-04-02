/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_vars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 13:08:11 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/02 18:37:35 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		handle_vars(t_cmd cmd, char ***vars);

/* adds the var to the var_list or reads the var.
TODO:
	1. replacement of old value in exists.	DONE
	2. expand_string function.				DONE */
int	handle_vars(t_cmd cmd, char ***vars)
{
	int		index;
	int		eq;
	char	name[MAX_NAME];

	//ft_printf("cmd=%s\n", cmd.words[0]);
	eq = ft_strichr(cmd.words[0], '=');
	if (eq <= 1 || eq > MAX_NAME || !vstr_name_is_valid(cmd.words[0]))
		return (0);
	index = mtx_getindex(vstr_getname(cmd.words[0], name, MAX_NAME), vars[1]);
	if (index >= 0)
	{
		vars[1] = mtx_vstr_copy(cmd.words[0], vars[1]);
		//vars[2] = mtx_vstr_copy(cmd.words[0], vars[2]);
	}
	else
		vars[0] = mtx_vstr_copy(cmd.words[0], vars[0]);
	//ft_printf("new variable: '%s'\n", cmd.words[0]);
	return (1);
}
