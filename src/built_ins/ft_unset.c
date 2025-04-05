/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totommi <totommi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 20:31:16 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/05 18:21:42 by totommi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_unset(int *fd, t_cmd cmd, char ***vars);

static void	mass_drop(char ***vars, char *var)
{
	char	name[MAX_NAME];
	int		index;
	int		i;
	
//	ft_printf("dropping: %s, buff (%p)\n", var, name);
	vstr_getname(var, name, MAX_NAME);
	i = 1;
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
	safeclose(fd[1]);
	return (errno);
}
