/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 20:30:38 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/01 18:35:56 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_export(int *fd, t_cmd cmd, char ***vars);

static void	sort_export(char **exp)
{
	int				sorted;
	char			*tmp;
	unsigned int	i;

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

static void	print_export(int *fd, char **exp)
{
	int		i;
	int		eq;
	
	sort_export(exp);
	i = 1;
	while (exp && exp[i] != NULL)
	{
		eq = ft_strichr(exp[i], '=');
		if (eq != 0)
			ft_printfd(fd[1], "declare -x %z\"%s\"\n", exp[i], eq, exp[i] + eq);
		else
			ft_printfd(fd[1], "declare -x %s\n", exp[i]);
		i++;
	}
}

//env no, shv no
//env no, shv si
//env si, shv no
//env si, shv si
int	ft_export(int *fd, t_cmd cmd, char ***vars)
{
	int		index;
	int		i;
	int		errno;
	char	name[MAX_NAME];

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
		if (!vstr_name_is_valid(cmd.words[i]))
		{
			ft_printfd(STDERR_FILENO, "minishell: export: `%s': not a valid identifier\n", cmd.words[i]);
			errno = 1;
		}
		else if (ft_strichr(cmd.words[i], '=') != 0)
		{
			vars[1] = mtx_vstr_copy(cmd.words[i], vars[1]);
			vars[2] = mtx_vstr_copy(cmd.words[i], vars[2]);
		}
		else
		{
			index = mtx_getindex(vstr_getname(cmd.words[i], name, MAX_NAME), vars[0]);
			if (index >= 0)
			{
				vars[1] = mtx_vstr_copy(vars[0][index], vars[1]); //remember to drop_index on shv
				mtx_safedel(index, vars[0]);
			}
			else
				vars[1] = mtx_vstr_copy(cmd.words[i], vars[1]);
		}
		i++;
	}
	return (errno);
}
/* RETURNS: 1 if all good, 0 in case of error. */
// int	ft_export(int *fd, t_cmd cmd, char ***vars)
// {
// 	int		index;
// 	int		i;
// 	int		errno;

// 	safeclose(fd[0]);
// 	if (!cmd.words[1] || cmd.words[1][0] == '\0')
// 	{
// 		print_export(fd, vars);
// 		safeclose(fd[1]);
// 		return (0);
// 	}
// 	safeclose(fd[1]);
// 	errno = 0;
// 	i = 1;
// 	while (cmd.words[i] && cmd.words[i][0] != '\0')
// 	{
// 		if (!var_check(cmd.words[i]))
// 		{
// 			ft_printfd(STDERR_FILENO, "minishell: export: `%s': not a valid identifier\n", cmd.words[i]);
// 			errno = 1;
// 		}
// 		else if (ft_strichr(cmd.words[i], '=') != 0)
// 		{
// 			vars[1] = var_append(vars[1], cmd.words[i]);
// 			vars[2] = var_append(vars[2], cmd.words[i]);
// 		}
// 		else
// 		{
// 			index = is_there((const char **)(vars[0] + 1), cmd.words[i]);
// 			if (index >= 0)
// 			{
// 				vars[1] = var_append(vars[1], vars[0][index]); //remember to drop_index on shv
// 				vars[0] = (char **)drop_index((void **)vars[0], index);
// 			}
// 			else
// 				vars[1] = var_append(vars[1], cmd.words[i]);		//appends the var without '='
// 		}
// 		i++;
// 	}
// 	return (errno);
// }
