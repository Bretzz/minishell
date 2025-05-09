/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 20:30:38 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/28 17:48:12 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "built_ins.h"

int	ft_export(int *fd, t_cmd cmd, char ***vars);

/* Adds the varstr to both exp and env, regardless if is present or not.
Then removes it from shv. */
static void	overwrite(char *varstr, char ***vars)
{
	char	*name;
	int		index;

	vars[1] = mtx_vstr_copy(varstr, vars[1]);
	name = vstr_getname(varstr, NULL, 0);
	if (name == NULL)
	{
		write(STDERR_FILENO, "minishell: malloc failure\n", 26);
		return ;
	}
	index = mtx_getindex(name, vars[0]);
	if (index >= 0)
		mtx_safedel(index, vars[0]);
	free(name);
}

/* Checks if the variable is present in exp, if it is does nothing.
Otherwise checks if it's in shv, in that case adds it to exp and env 
and removes it from shv. */
static void	rank_up(char *varstr, char ***vars)
{
	char	*name;
	int		index;

	name = vstr_getname(varstr, NULL, 0);
	if (name == NULL)
	{
		write(STDERR_FILENO, "minishell: malloc failure\n", 26);
		return ;
	}
	index = mtx_getindex(name, vars[1]);
	if (index >= 0)
	{
		free(name);
		return ;
	}
	index = mtx_getindex(name, vars[0]);
	if (index >= 0)
	{
		vars[1] = mtx_vstr_copy(vars[0][index], vars[1]);
		mtx_safedel(index, vars[0]);
	}
	else
		vars[1] = mtx_setval(name, NULL, vars[1]);
	free(name);
}

static void	loop_export(t_cmd cmd, int *exit_code, char ***vars)
{
	int	i;

	i = 1;
	while (cmd.words[i] && cmd.words[i][0] != '\0')
	{
		if (vstr_name_is_valid(cmd.words[i]) <= 0)
		{
			ft_printfd(STDERR_FILENO, "minishell: export: `%s': not a \
valid identifier\n", cmd.words[i]);
			*exit_code = 1;
		}
		else if (ft_strichr(cmd.words[i], '=') != 0)
			overwrite(cmd.words[i], vars);
		else
			rank_up(cmd.words[i], vars);
		i++;
	}
}

//env no, shv no
//env no, shv si
//env si, shv no
//env si, shv si
int	ft_export(int *fd, t_cmd cmd, char ***vars)
{
	int	exit_code;

	if ((cmd.redir[1] == FILE && cmd.fd[1] < 0)
		|| (cmd.redir[0] == FILE && cmd.fd[0] < 0))
		return (safeclose(fd[1]), 1);
	if (!cmd.words[1] || cmd.words[1][0] == '\0')
	{
		print_export(fd[1], vars[1]);
		return (safeclose(fd[1]), 0);
	}
	safeclose(fd[1]);
	exit_code = 0;
	loop_export(cmd, &exit_code, vars);
	return (exit_code);
}
