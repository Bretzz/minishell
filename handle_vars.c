/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_vars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 13:08:11 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/10 14:47:45 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_vars(char *cmd, char ***sh_vars, char **__environ);

/* checks weather the target sh_var is present or not.
RETURNS: the index of the var, -1 if not present or sh_vars/target is NULL. */
int	is_there(const char **sh_vars, const char *target)
{
	char	*my_tar;
	size_t	tar_len;
	int		i;

	if (sh_vars == NULL || target == NULL)
		return (-1);
	my_tar = ft_strdup(target);
	tar_len = ft_strlen(my_tar);
	my_tar[tar_len] = '=';
	i = 0;
	while (sh_vars[i] != NULL)
	{
		if (!ft_strncmp(sh_vars[i], my_tar, tar_len + 1))
		{
			free(my_tar);
			return (i);
		}
		i++;
	}
	free(my_tar);
	return(-1);
}

/* we're expecting to find an sh_var with same key as var at the index 'index',
so we free that string and replace it with var.
RETURNS: a pointer to the matrix, NULL on malloc failure or NULL sh_vars. */
char	**replace_var(int index, char **sh_vars, char *var)
{

	if (sh_vars == NULL)
		return (NULL);
	free(sh_vars[index]);
	sh_vars[index] = ft_strdup(var);
	if (sh_vars[index] == NULL)
	{
		ft_free_arr((void **)sh_vars);
		return (NULL);
	}
	ft_print_charr((const char **)sh_vars);
	return (sh_vars);
}

/*
var1=123
var2=laskd
var3=
var4=123;12k3
*/
char	*get_value(const char **sh_vars, const char *target)
{
	size_t	tar_len;
	size_t	var_len;
	char	*my_tar;
	int		i;
	
	if (sh_vars == NULL || target == NULL)
		return (NULL);
	my_tar = ft_strdup(target);
	tar_len = ft_strlen(my_tar);
	my_tar[tar_len] = '=';
	i = 0;
	while (sh_vars[i] != NULL)
	{
		if (!ft_strncmp(sh_vars[i], my_tar, tar_len + 1))
		{
			free(my_tar);
			var_len = ft_strlen(sh_vars[i]) - (tar_len + 1);
			return (ft_substr(sh_vars[i], tar_len + 1, var_len));
		}
		i++;
		/* while (*vars != '\0')
		{
			if (*(vars++) == '\n')
				break ;
		} */
	}
	free(my_tar);
	return (NULL);
}

/* adds a new var_string allocated with malloc(3) to the sh_vars matrix.
If the var_string is NULL the var_append just returns the ptr to the matrix. 
var_string syntax: varname=varvalue.
NOTE: the var_string must be a NULL-terminated string.
RETURNS: a pointer to the head of the matrix, NULL if a malloc fails. */
char	**var_append(char **sh_vars, char *var)
{
	char	**new_matrix;
	size_t	len;
	int		index;

	ft_print_charr((const char **)sh_vars);
	if (var == NULL)
		return (sh_vars);
	len = ft_mtxlen((void *)sh_vars);
	ft_printf("len=%i\n", len);
	index = is_there((const char **)sh_vars, var);
	if (index >= 0)
		return (replace_var(index, sh_vars, var));
	new_matrix = ft_realloc(sh_vars, len * sizeof(char *), (len + 2) * sizeof(char *));
	//new_matrix = realloc(sh_vars, (len + 2) * sizeof(char *));
	if (new_matrix != NULL)
	{
		new_matrix[len + 1] = NULL;
		ft_printf("post realloc:\n");
		ft_print_charr((const char **)new_matrix);
		new_matrix[len] = ft_strdup(var);
		if (new_matrix[len] != NULL)
		{
			ft_free_arr((void **)sh_vars);
			ft_printf("final arr:\n");
			ft_print_charr((const char **)new_matrix);
			return (new_matrix);
		}
	}
	ft_free_arr((void **)sh_vars);
	ft_free_arr((void **)new_matrix);
	return (NULL);
}

/* adds the var to the var_list or reads the var.
TODO:
	1. replacement of old value in exists.
	2. expand_string function. */
void	handle_vars(char *cmd, char ***sh_vars, char **__environ)
{
	char	*eq;

	(void)__environ;
	//ft_printf("cmd=%s\n", cmd);
	eq = ft_strchr(cmd, '=');
	//ft_printf("eq=%s\n", eq);
	if (eq != NULL && eq != cmd)
		*sh_vars = var_append(*sh_vars, cmd);
		/* *shell_var = ft_strjoin_free_space_nl(*shell_var, cmd); */
	else if (!ft_strncmp("$", cmd, 1))
	{
		ft_printf("%s\n", get_value((const char **)*sh_vars, ft_strchr(cmd, '$') + 1));
	}
	return ;
}
