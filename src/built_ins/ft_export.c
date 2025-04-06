/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 20:30:38 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/06 16:12:45 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_export(int *fd, t_cmd cmd, char ***vars);

static void	print_var_export(int fd, const char *varstr)
{
	int	eq;

	eq = ft_strichr(varstr, '=');
	if (eq != 0)
		ft_printfd(fd, "declare -x %z\"%s\"\n", varstr, eq, varstr + eq);
	else
		ft_printfd(fd, "declare -x %s\n", varstr);
}

/* takes an int array of size 'len' and a mtx-matrix 'exp'.
Cycles trough evey element of exp to find the ones with lower ascii value
(whole 'name=value' checked) and the correspondig index in 'printed' is 0.
RETUENS: the index found, -1 if the string in 'exp' with that index is NULL. */
static int	get_best_ascii(int *printed, unsigned int len, char **exp)
{
	unsigned int	best_ascii;
	unsigned int	i;
	
	best_ascii = 1;
	while (best_ascii < len && printed[best_ascii] != 0)
		best_ascii++;
	if (exp[best_ascii] == NULL)
		return (-1);
	i = 1;
	while (exp[i] != NULL)
	{
		if (!printed[i] && ft_strncmp(exp[i], exp[best_ascii], ft_strlen(exp[i])) < 0)
			best_ascii = i;
		i++;
	}
	return (best_ascii);
}

static void	print_sort_export(int fd, char **exp)
{
	int				best_ascii;
	unsigned int	len;
	int				all_done;
	int				*printed;
	
	len = ft_mtxlen((const void **)exp);
	printed = (int *)ft_calloc(len, sizeof(int));
	if (printed == NULL)
	{
		write(STDERR_FILENO, "minishell: malloc failure\n", 26);
		return ;
	}
	all_done = len;
	while (all_done > 0)
	{
		best_ascii = get_best_ascii(printed, len, exp);
		if (best_ascii < 0)
			break ;
		print_var_export(fd, exp[best_ascii]);
		printed[best_ascii] = 1;
		all_done--;
	}
	free(printed);
}

/* Adds the varstr to both exp and env, regardless if is present or not.
Then removes it from shv. */
static void	overwrite(char *varstr, char ***vars)
{
	char	name[MAX_NAME];
	int		index;

	vars[1] = mtx_vstr_copy(varstr, vars[1]);
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
		mtx_safedel(index, vars[0]);
	}
	else
		vars[1] = mtx_setval(name, NULL, vars[1]);
}

//env no, shv no
//env no, shv si
//env si, shv no
//env si, shv si
int	ft_export(int *fd, t_cmd cmd, char ***vars)
{
	int		exit_status;
	int		i;

	if (!cmd.words[1] || cmd.words[1][0] == '\0')
	{
		print_sort_export(fd[1], vars[1]);
		safeclose(fd[1]);
		return (0);
	}
	safeclose(fd[1]);
	exit_status = 0;
	i = 1;
	while (cmd.words[i] && cmd.words[i][0] != '\0')
	{
		if (!vstr_name_is_valid(cmd.words[i]))
		{
			ft_printfd(STDERR_FILENO, "minishell: export: `%s': not a valid identifier\n", cmd.words[i]);
			exit_status = 1;
		}
		else if (ft_strichr(cmd.words[i], '=') != 0)
			overwrite(cmd.words[i], vars);
		else
			rank_up(cmd.words[i], vars);
		i++;
	}
	return (exit_status);
}
