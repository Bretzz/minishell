/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 20:31:51 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/21 14:14:48 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(int *fd, const char ***vars);

int	ft_env(int *fd, const char ***vars)
{
	int	i;

	if (vars[2] == NULL)	//should never happen
	{
		ft_printfd(STDERR_FILENO, "%p\n", vars[2]);
		multicose(fd);
		return (-1);
	}
	i = 0;
	while (vars[2][i] != NULL)
	{
		ft_printfd(fd[1], "%s\n", vars[2][i]);
		i++;
	}
	multicose(fd);
	return (0);
}
