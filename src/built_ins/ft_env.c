/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 20:31:51 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/18 22:26:19 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(int *fd, char **env);

int	ft_env(int *fd, char **env)
{
	int	i;

	if (env == NULL)
	{
		ft_printfd(STDERR_FILENO, "%p\n", env); //should never happen
		multicose(fd);
		return (-1);
	}
	i = 0;
	while (env[i] != NULL)
	{
		ft_printfd(fd[1], "%s\n", env[i]);
		i++;
	}
	multicose(fd);
	return (0);
}
