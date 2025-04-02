/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 20:31:16 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/02 14:43:51 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_unset(int *fd, t_cmd cmd, char ***vars);

static void	mass_drop(char ***vars, char *var)
{
	int	index;
	int	i;
	char	name[MAX_NAME];
	
	vstr_getname(var, name, MAX_NAME);
	i = 2;
	while (i >= 0)
	{
		if (vars[i] != NULL)
		{
			index = mtx_getindex(name, vars[i]);
			if (index >= 0)
				mtx_safedel(index, vars[i]);
		}
		i--;
	}
}

int	ft_unset(int *fd, t_cmd cmd, char ***vars)
{
	int		errno;
	int		i;

	multicose(fd);
	errno = 0;
	i = 1;
	while (cmd.words[i] && cmd.words[i][0] != '\0')
	{
		if (!vstr_name_is_valid(cmd.words[i]))
		{
			ft_printfd(STDERR_FILENO, "minishell: unset: `%s': not a valid identifier\n", cmd.words[i]);
			errno = 1;
		}
		else
			mass_drop(vars, cmd.words[i]);
		i++;
	}
	return (errno);
}
