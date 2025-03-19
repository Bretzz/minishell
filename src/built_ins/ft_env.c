/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totommi <totommi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 20:31:51 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/19 03:11:44 by totommi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(int *fd, const char ***vars);

int	ft_env(int *fd, const char ***vars)
{
	int	i;

	if (vars[2] == NULL)
	{
		ft_printfd(STDERR_FILENO, "%p\n", vars[2]); //should never happen
		multicose(fd);
		return (-1);
	}
	i = 0;
	while (vars[2][i] != NULL)
	{
		ft_printfd(fd[1], "%s\n", vars[2][i]);
		i++;
	}
	i = 0;
	while (vars[1] && vars[1][i] != NULL)
	{
		if (ft_strichr(vars[1][i], '=') != 0)
			ft_printfd(fd[1], "%s\n", vars[1][i]);
		i++;
	}
	multicose(fd);
	return (0);
}
