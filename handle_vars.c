/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_vars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 13:08:11 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/10 21:01:44 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		handle_vars(char *cmd, char ***shv, char **env);

/* checks weather the target sh_var is present or not.
RETURNS: the index of the var, -1 if not present or shv/target is NULL. */
int	is_there(const char **shv, const char *target)
{
	char	*my_tar;
	size_t	tar_len;
	int		i;

	if (shv == NULL || target == NULL)
		return (-1);
	i = 0;
	while (target[i] && target[i] != '=')
		i++;
	// if (*target == '$')	//both $var and var=value behaviour should be impelented
		//code here
	my_tar = ft_substr(target, 0, i);
	tar_len = ft_strlen(my_tar);
	i = 0;
	while (shv[i] != NULL)
	{
		if (!ft_strncmp(shv[i], my_tar, tar_len))
			return (free(my_tar), i);
		i++;
	}
	free(my_tar);
	return(-1);
}

/* we're expecting to find an sh_var with same key as var at the index 'index',
so we free that string and replace it with var.
RETURNS: a pointer to the matrix, NULL on malloc failure or NULL shv. */
char	**replace_var(int index, char **shv, char *var)
{
	if (shv == NULL)
		return (NULL);
	free(shv[index]);
	shv[index] = ft_strdup(var);
	if (shv[index] == NULL)
	{
		ft_free_arr((void **)shv);
		return (NULL);
	}
//	ft_print_charr((const char **)shv);
	return (shv);
}

/* adds a new var_string allocated with malloc(3) to the shv matrix.
If the var_string is NULL the var_append just returns the ptr to the matrix. 
var_string syntax: varname=varvalue.
NOTE: the var_string must be a NULL-terminated string.
RETURNS: a pointer to the head of the matrix, NULL if a malloc fails. */
char	**var_append(char **shv, char *var)
{
	char	**new_matrix;
	size_t	len;
	int		index;

//	ft_print_charr((const char **)shv);
	if (var == NULL)
		return (shv);
	len = ft_mtxlen((void *)shv);
	index = is_there((const char **)shv, var);
	if (index >= 0)
		return (replace_var(index, shv, var));
	new_matrix = ft_realloc(shv, len * sizeof(char *), (len + 2) * sizeof(char *));
	if (new_matrix != NULL)
	{
		new_matrix[len + 1] = NULL;
		new_matrix[len] = ft_strdup(var);
		if (new_matrix[len] != NULL)
		{
			free(shv);
//			ft_print_charr((const char **)new_matrix);
			return (new_matrix);
		}
	}
	ft_free_arr((void **)shv);
	ft_free_arr((void **)new_matrix);
	return (NULL);
}

/* adds the var to the var_list or reads the var.
TODO:
	1. replacement of old value in exists. DONE
	2. expand_string function. */
int	handle_vars(char *cmd, char ***shv, char **env)
{
	char	*eq;

	(void)env;
	//ft_printf("cmd=%s\n", cmd);
	eq = ft_strchr(cmd, '=');
	//ft_printf("eq=%s\n", eq);
	if (eq != NULL && eq != cmd)
	{
		*shv = var_append(*shv, cmd);
		return (1);
	}
		/* *shell_var = ft_strjoin_free_space_nl(*shell_var, cmd); */
	else if (!ft_strncmp("$", cmd, 1))
	{
		ft_printf("%s\n", get_value(ft_strchr(cmd, '$') + 1, (const char **)*shv, (const char **)env));
		return (1);
	}
	return (0);
}
