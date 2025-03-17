/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_vars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapascal <mapascal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 13:08:11 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/17 17:42:08 by mapascal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		handle_vars(t_cmd cmd, char ***shv, char ***env);
char	**var_append(char **mtx, char *var);
int		is_there(const char **mtx, const char *target);

/* checks weather the target var is present or not.
RETURNS: the index of the var, -1 if not present or mtx/target is NULL. */
int	is_there(const char **mtx, const char *target)
{
	char	*my_tar;
	size_t	tar_len;
	int		i;

	if (mtx == NULL || target == NULL)
		return (-1);
	i = 0;
	while (target[i] && target[i] != '=')
		i++;
	// if (*target == '$')	//both $var and var=value behaviour should be impelented
		//code here
	my_tar = ft_substr(target, 0, i);
	tar_len = ft_strlen(my_tar);
	i = 0;
	while (mtx[i] != NULL)
	{
		if (!ft_strncmp(mtx[i], my_tar, tar_len))
			return (free(my_tar), i);
		i++;
	}
	free(my_tar);
	return(-1);
}

/* we're expecting to find an mtx_var with same key as var at the index 'index',
so we free that string and replace it with var.
RETURNS: a pointer to the matrix, NULL on malloc failure or NULL mtx. */
static char	**replace_var(int index, char **mtx, char *var)
{
	if (mtx == NULL)
		return (NULL);
	free(mtx[index]);
	mtx[index] = ft_strdup(var);
	if (mtx[index] == NULL)
	{
		ft_printfd(2, "malloc failre\n");
		//ft_freentf("2", mtx);
		return (NULL);
	}
//	ft_print_charr((const char **)mtx);
	return (mtx);
}

/* adds a new var_string allocated with malloc(3) to the passed matrix.
If the var_string is NULL the var_append just returns the ptr to the matrix. 
var_string syntax: varname=varvalue.
NOTE: the var_string must be a NULL-terminated string.
RETURNS: a pointer to the head of the matrix, NULL if a malloc fails. */
char	**var_append(char **mtx, char *var)
{
	char	**new_matrix;
	size_t	len;
	int		index;

//	ft_print_charr((const char **)mtx);
	if (var == NULL)
		return (mtx);
	len = ft_mtxlen((void *)mtx);
	index = is_there((const char **)mtx, var);
	if (index >= 0)
		return (replace_var(index, mtx, var));
	new_matrix = ft_realloc(mtx, len * sizeof(char *), (len + 2) * sizeof(char *));
	if (new_matrix != NULL)
	{
		new_matrix[len] = ft_strdup(var);
		if (new_matrix[len] != NULL)
		{
			free(mtx);
			new_matrix[len + 1] = NULL;
			//			ft_print_charr((const char **)new_matrix);
			return (new_matrix);
		}
	}
	ft_freentf("22", mtx, new_matrix);
	/* ft_free_arr((void **)mtx);
	ft_free_arr((void **)new_matrix); */
	return (NULL);
}

/* adds the var to the var_list or reads the var.
TODO:
	1. replacement of old value in exists. DONE
	2. expand_string function. */
int	handle_vars(t_cmd cmd, char ***shv, char ***env)
{
	char	*eq;
	char	**mtx;
	int		index;

	(void)env;
	//ft_printf("cmd=%s\n", cmd);
	eq = ft_strchr(cmd.words[0], '=');
	//ft_printf("eq=%s\n", eq);
	if (eq != NULL && eq != cmd.words[0])
	{
		index = is_there((const char **)*env, cmd.words[0]);
		if (index < 0)
			mtx = var_append(*shv, cmd.words[0]);
		else
			mtx = var_append(*env, cmd.words[0]);
		if (mtx == NULL)
			return (0);
		if (index < 0)
			*shv = mtx;
		else
			*env = mtx;
		return (1);
	}
		/* *shell_var = ft_strjoin_free_space_nl(*shell_var, cmd); */
	// else if (!ft_strncmp("$", cmd.words[0], 1))
	// {
	// 	ft_printf("%s\n", expand_string(cmd.words[0], (const char **)*shv, (const char **)*env));
	// 	return (1);
	// }
	return (0);
}
