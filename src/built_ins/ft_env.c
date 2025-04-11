/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 20:31:51 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/11 14:11:29 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "built_ins.h"

int	ft_env(int *fd, const char ***vars);

int	ft_env(int *fd, const char ***vars)
{
	unsigned int	i;
	char			*value;

	if (vars[1] == NULL)
	{
		ft_printfd(STDERR_FILENO, "%p\n", vars[1]);
		safeclose(fd[1]);
		return (-1);
	}
	i = 1;
	while (vars[1][i] != NULL)
	{
		value = vstr_getvalue((char *)vars[1][i], NULL, 0);
		if (value != NULL)
			ft_printfd(fd[1], "%s\n", vars[1][i]);
		free(value);
		i++;
	}
	safeclose(fd[1]);
	return (0);
}
