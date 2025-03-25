/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 20:31:16 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/25 18:13:37 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_unset(int *fd, t_cmd cmd, char ***vars);

static void	mass_drop(char ***vars, char *var)
{
	int	index;
	int	i;
	
	i = 2;
	while (i >= 0)
	{
		if (vars[i] != NULL)
		{
			if (i == 0) //skips LITTLEPIPE
				index = is_there((const char **)(vars[i] + 1), var);
			else
				index = is_there((const char **)(vars[i]), var);
			if (index >= 0)
				vars[i] = (char **)drop_index((void **)vars[i], index);
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
		if (!var_check(cmd.words[i]))
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
