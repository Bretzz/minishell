/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 20:31:51 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/02 17:34:59 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(int *fd, const char ***vars);

int	ft_env(int *fd, const char ***vars)
{
	unsigned int	i;
	char			buff[2];

	if (vars[1] == NULL)	//should never happen
	{
		ft_printfd(STDERR_FILENO, "%p\n", vars[1]);
		multicose(fd);
		return (-1);
	}
	i = 1;
	while (vars[1][i] != NULL)
	{
		if (vstr_getvalue((char *)vars[1][i], buff, sizeof(buff)) != NULL)
			ft_printfd(fd[1], "%s\n", vars[1][i]);
		i++;
	}
	multicose(fd);
	return (0);
}
