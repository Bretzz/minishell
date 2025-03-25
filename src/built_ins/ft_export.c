/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 20:30:38 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/25 18:11:33 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_export(int *fd, t_cmd cmd, char ***vars);

static void	print_export(int *fd, char ***vars)
{
	int		i;
	int		eq;
	
	// i = 0;
	// while (vars[2] && vars[2][i] != NULL)
	// {
	// 	eq = ft_strichr(vars[2][i], '=');
	// 	ft_printfd(fd[1], "declare -x %z\"%s\"\n", vars[2][i], eq, vars[2][i] + eq);
	// 	i++;
	// }
	i = 0;
	while (vars[1] && vars[1][i] != NULL)
	{
		eq = ft_strichr(vars[1][i], '=');
		if (eq != 0)
			ft_printfd(fd[1], "declare -x %z\"%s\"\n", vars[1][i], eq, vars[1][i] + eq);
		else
			ft_printfd(fd[1], "declare -x %s\n", vars[1][i]);
		i++;
	}
}

//env no, shv no
//env no, shv si
//env si, shv no
//env si, shv si
/* RETURNS: 1 if all good, 0 in case of error. */
int	ft_export(int *fd, t_cmd cmd, char ***vars)
{
	int		index;
	int		i;
	int		errno;

	safeclose(fd[0]);
	if (!cmd.words[1] || cmd.words[1][0] == '\0')
	{
		print_export(fd, vars);
		safeclose(fd[1]);
		return (0);
	}
	safeclose(fd[1]);
	errno = 0;
	i = 1;
	while (cmd.words[i] && cmd.words[i][0] != '\0')
	{
		if (!var_check(cmd.words[i]))
		{
			ft_printfd(STDERR_FILENO, "minishell: export: `%s': not a valid identifier\n", cmd.words[i]);
			errno = 1;
		}
		else if (ft_strichr(cmd.words[i], '=') != 0)
		{
			vars[1] = var_append(vars[1], cmd.words[i]);
			vars[2] = var_append(vars[2], cmd.words[i]);
		}
		else
		{
			index = is_there((const char **)(vars[0] + 1), cmd.words[i]);
			if (index >= 0)
			{
				vars[1] = var_append(vars[1], vars[0][index]); //remember to drop_index on shv
				vars[0] = (char **)drop_index((void **)vars[0], index);
			}
			else
				vars[1] = var_append(vars[1], cmd.words[i]);		//appends the var without '='
		}
		i++;
	}
	return (errno);
}
