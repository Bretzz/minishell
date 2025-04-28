/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 20:31:51 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/28 17:45:21 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "built_ins.h"

int	ft_env(int *fd, t_cmd cmd, const char ***vars);

int	ft_env(int *fd, t_cmd cmd, const char ***vars)
{
	unsigned int	i;
	char			*value;

	if ((cmd.redir[1] == FILE && cmd.fd[1] < 0)
		|| (cmd.redir[0] == FILE && cmd.fd[0] < 0))
		return (safeclose(fd[1]), 1);
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
