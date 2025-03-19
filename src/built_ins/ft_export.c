/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totommi <totommi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 20:30:38 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/19 04:08:37 by totommi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_export(int *fd, t_cmd cmd, char ***vars);

/* NOTE: I'd like to avoid using split */
/* static void	print_export(int *fd, char ***vars)
{
	int		i;
	char	**split;
	
	i = 0;
	while (vars[2] && vars[2][i] != NULL)
	{
		split = ft_split(vars[2][i], '=');
		ft_printfd(fd[1], "declare -x %s=\"%s\"\n", split[0], split[1]);
		ft_freentf("2", split);
		i++;
	}
	i = 0;
	while (vars[1] && vars[1][i] != NULL)
	{
		if (ft_strichr(vars[1][i], '=') != 0)
		{
			split = ft_split(vars[1][i], '=');
			ft_printfd(fd[1], "declare -x %s=\"%s\"\n", split[0], split[1]);
			ft_freentf("2", split);
		}
		else
			ft_printfd(fd[1], "declare -x %s\n", vars[1][i]);
		i++;
	}
} */

static void	print_export(int *fd, char ***vars)
{
	int		i;
	int		eq;
	char	var[MAX_VAR_LEN];
	char	value[MAX_VAL_LEN];
	
	i = 0;
	while (vars[2] && vars[2][i] != NULL)
	{
		ft_bzero(var, MAX_VAR_LEN);
		ft_bzero(value, MAX_VAL_LEN);
		eq = ft_strichr(vars[2][i], '=');
		ft_strlcpy(var, vars[2][i], eq);
		ft_strlcpy(value, vars[2][i] + eq, ft_strlen(vars[2][i] + eq) + 1);
		ft_printfd(fd[1], "declare -x %s=\"%s\"\n", var, value);
		i++;
	}
	i = 0;
	while (vars[1] && vars[1][i] != NULL)
	{
		ft_bzero(var, MAX_VAR_LEN);
		ft_bzero(value, MAX_VAL_LEN);
		eq = ft_strichr(vars[1][i], '=');
		if (eq != 0)
		{
			ft_strlcpy(var, vars[1][i], eq);
			ft_strlcpy(value, vars[1][i] + eq, ft_strlen(vars[1][i] + eq) + 1);
			ft_printfd(fd[1], "declare -x %s=\"%s\"\n", var, value);
		}
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
			errno = 1;
			i++;
			continue ;
		}
		if (ft_strichr(cmd.words[i], '=') != 0)
		{
			vars[1] = var_append(vars[1], cmd.words[i]);
			i++;
			continue ;
		}
		index = is_there((const char **)vars[0], cmd.words[i]);
		if (index >= 0)
		{
			vars[1] = var_append(vars[1], vars[0][index]); //remember to drop_index on shv
			vars[0] = (char **)drop_index((void **)vars[0], index);
			i++;
			continue ;
		}
		//ft_printf("adding monkerino to export\n");
		vars[1] = var_append(vars[1], cmd.words[i]);		//appends the var without '='
		i++;
	}
	return (errno);
}
