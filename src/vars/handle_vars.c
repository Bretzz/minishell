/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_vars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 13:08:11 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/15 14:47:52 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		handle_vars(t_cmd cmd, char ***vars);

/* adds the var to the var_list. */
int	handle_vars(t_cmd cmd, char ***vars)
{
	int		index;
	int		eq;
	char	*name;

	eq = ft_strichr(cmd.words[0], '=');
	if (eq <= 1 || vstr_name_is_valid(cmd.words[0]) <= 0)
		return (0);
	name = vstr_getname(cmd.words[0], NULL, 0);
	if (name == NULL)
	{
		write(STDERR_FILENO, "minishell: malloc failure\n", 26);
		return (1);
	}
	index = mtx_getindex(name, vars[1]);
	if (index >= 0)
	{
		vars[1] = mtx_vstr_copy(cmd.words[0], vars[1]);
	}
	else
		vars[0] = mtx_vstr_copy(cmd.words[0], vars[0]);
	free(name);
	return (1);
}
