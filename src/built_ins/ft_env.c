/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totommi <totommi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 20:31:51 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/05 18:27:21 by totommi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(int *fd, const char ***vars);

int	ft_env(int *fd, const char ***vars)
{
	unsigned int	i;
	char			*value;

	if (vars[1] == NULL)	//should never happen
	{
		ft_printfd(STDERR_FILENO, "%p\n", vars[1]);
		safeclose(fd[1]);
		return (-1);
	}
	i = 1;
	while (vars[1][i] != NULL)
	{
		value = vstr_getvalue((char *)vars[1][i], NULL, 0);
		//if (vstr_getvalue((char *)vars[1][i], buff, sizeof(buff)) != NULL)
		if (value != NULL)
			ft_printfd(fd[1], "%s\n", vars[1][i]);
		free(value);
		i++;
	}
	safeclose(fd[1]);
	return (0);
}
