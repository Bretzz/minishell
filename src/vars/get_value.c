/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_value.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totommi <totommi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 20:12:08 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/19 03:46:17 by totommi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_value(const char *target, const char **mtx);
int		var_is_valid(const char *var);
int		var_check(char *var);

/* just a var_is_valid() wrapper.  */
int	var_check(char *var)
{
	char	my_var[MAX_VAR_LEN];
	int		eq;
	
	eq = ft_strichr(var, '=');
	if (eq == 1 || eq > MAX_VAR_LEN)
		return (0);
	//ft_printf("no = foound\n");
	ft_bzero(my_var, MAX_VAR_LEN);
	if (eq != 0)
		ft_strlcpy(my_var, var, eq + 1);
	else
		ft_strlcpy(my_var, var, ft_strlen_space(var) + 1);
	if (!var_is_valid(my_var))
		return (0);
	return (1);
}

/* checks if the var is a valid name:
	no spaces (neither leading nor trailing)
	only alphabetical chars (upper or lowe)
	only separator allowed: '_'
RETURNS: 1 if the var is valid, 0 if it isn't. */
int	var_is_valid(const char *var)
{
	int	i;

	if (var == NULL)
		return (0);
	i = 0;
	while (var[i] != '\0')
	{
		if (i == 0 && !ft_isalpha(var[i]))
		{
			ft_printfd(STDERR_FILENO, "minishell: export: `%s': not a valid identifier\n", var);
			return (0);
		}
		if (!ft_isalpha(var[i]) && !ft_isdigit(var[i]) && var[i] == '_')
		{
			ft_printfd(STDERR_FILENO, "minishell: export: `%s': not a valid identifier\n", var);
			return (0);
		}
		i++;
	}
	return (1);
}

/*
var1=123
var2=laskd
var3=
var4=123;12k3
NOTE: target: 'var' without the '='*/
char	*get_value(const char *target, const char **mtx)
{
	size_t	tar_len;
	char	*my_tar;
	int		index;
	
	if (target == NULL || mtx == NULL)
		return (NULL);
	tar_len = ft_strlen_space(target); //just for flexible usage
	my_tar = ft_substr(target, 0, tar_len);
	if (!var_is_valid(my_tar))
	{
		//throw error? (treated as a cmd (or a path if contains '/'))
		return (NULL);
	}
	index = is_there(mtx, my_tar);
	if (index < 0)
		return (NULL);
	free(my_tar);
	return(ft_substr(mtx[index], tar_len + 1, ft_strlen(mtx[index] + tar_len + 1)));
}
