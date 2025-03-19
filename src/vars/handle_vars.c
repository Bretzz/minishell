/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_vars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totommi <totommi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 13:08:11 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/19 02:53:29 by totommi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		handle_vars(t_cmd cmd, char ***vars);
char	**var_append(char **mtx, char *var);

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
RETURNS: a pointer to the head of the matrix, NULL if a malloc fails.
NOTE: is safe to assign the originale matrix to the return of var_append,
since if it fails somehow just returns the old matrix.
NOTE2: a valid var is expected, var_is_valid() function check. */
char	**var_append(char **mtx, char *var)
{
	char	**new_matrix;
	size_t	len;
	int		index;

//	ft_print_charr((const char **)mtx);
	if (var == NULL)
		return (mtx);
	len = ft_mtxlen((const void **)mtx);
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
			//ft_print_charr((const char **)new_matrix);
			return (new_matrix);
		}
	}
	//not freeing so the arr is still usable
	ft_printfd(STDERR_FILENO, "minishell: malloc error that maybe is handled\n");
	return (mtx);
}

/* adds the var to the var_list or reads the var.
TODO:
	1. replacement of old value in exists.	DONE
	2. expand_string function.				DONE */
int	handle_vars(t_cmd cmd, char ***vars)
{
	char	my_var[MAX_VAR_LEN];
	int		index;
	int		eq;
	int		i;

	//ft_printf("cmd=%s\n", cmd.words[0]);
	eq = ft_strichr(cmd.words[0], '=');
	if (eq <= 1 || eq > MAX_VAR_LEN)
		return (0);
	ft_bzero(my_var, MAX_VAR_LEN);
	ft_strlcpy(my_var, cmd.words[0], eq + 1);
	if (!var_is_valid(my_var))
		return (0);
	//ft_printf("var assignment found\n");
	i = 2;
	while (i > 0)
	{
		index = is_there((const char **)vars[i], cmd.words[0]);
		if (index >= 0)
			vars[i] = var_append(vars[i], cmd.words[0]);
		i--;
	}
	vars[0] = var_append(vars[0], cmd.words[0]);
	// if (!ft_strncmp("$?", cmd.words[0], 3))
	// {
	//	ft_printf("%d\n", g_pipe_status);
	//	return (1);
	// }
	return (1);
}
