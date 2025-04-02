/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 20:30:38 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/02 13:23:19 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_export(int *fd, t_cmd cmd, char ***vars);

static void	sort_export(char **exp)
{
	int				sorted;
	char			*tmp;
	int				i;

	sorted = 1;
	i = 1;
	while (i < *(int *)exp && exp[i] != NULL)
	{
		if (exp[i + 1]
			&& ft_strncmp(exp[i], exp[i + 1], ft_strlen(exp[i])) > 0)
		{
			tmp = exp[i];
			exp[i] = exp[i + 1];
			exp[i + 1] = tmp;
			sorted = 0;
		}
		i++;
		if (!sorted && (i == *(int *)exp || exp[i] == NULL))
		{
			sorted = 1;
			i = 1;
		}
	}
}

static void	print_export(int fd, char **exp)
{
	int		i;
	int		eq;
	
	sort_export(exp);
	i = 1;
	while (exp && exp[i] != NULL)
	{
		eq = ft_strichr(exp[i], '=');
		if (eq != 0)
			ft_printfd(fd, "declare -x %z\"%s\"\n", exp[i], eq, exp[i] + eq);
		else
			ft_printfd(fd, "declare -x %s\n", exp[i]);
		i++;
	}
}

/* Adds the varstr to both exp and env, regardless if is present or not.
Then removes it from shv. */
static void	overwrite(char *varstr, char ***vars)
{
	char	name[MAX_NAME];
	int		index;

	vars[1] = mtx_vstr_copy(varstr, vars[1]);
	vars[2] = mtx_vstr_copy(varstr, vars[2]);
	vstr_getname(varstr, name, MAX_NAME);
	index = mtx_getindex(name, vars[0]);
	if (index >= 0)
		mtx_safedel(index, vars[0]);
}

/* Checks if the variable is present in exp, if it is does nothing.
Otherwise checks if it's in shv, in that case adds it to exp and env and removes it from shv. */
static void	rank_up(char *varstr, char ***vars)
{
	char	name[MAX_NAME];
	int		index;

	vstr_getname(varstr, name, MAX_NAME);
	index = mtx_getindex(name, vars[1]);
	if (index >= 0)
		return ;
	index = mtx_getindex(name, vars[0]);
	if (index >= 0)
	{
		vars[1] = mtx_vstr_copy(vars[0][index], vars[1]);
		vars[2] = mtx_vstr_copy(vars[0][index], vars[2]);
		mtx_safedel(index, vars[0]);
	}
	else
		vars[1] = mtx_vstr_copy(varstr, vars[1]);
}

//env no, shv no
//env no, shv si
//env si, shv no
//env si, shv si
int	ft_export(int *fd, t_cmd cmd, char ***vars)
{
	int		i;
	int		errno;

	safeclose(fd[0]);
	if (!cmd.words[1] || cmd.words[1][0] == '\0')
	{
		print_export(fd[1], vars[1]);
		safeclose(fd[1]);
		return (0);
	}
	safeclose(fd[1]);
	errno = 0;
	i = 1;
	while (cmd.words[i] && cmd.words[i][0] != '\0')
	{
		if (!vstr_name_is_valid(cmd.words[i]))
		{
			ft_printfd(STDERR_FILENO, "minishell: export: `%s': not a valid identifier\n", cmd.words[i]);
			errno = 1;
		}
		else if (ft_strichr(cmd.words[i], '=') != 0)
			overwrite(cmd.words[i], vars);
		else
			rank_up(cmd.words[i], vars);
		i++;
	}
	return (errno);
}
