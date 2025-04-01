/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_value.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 20:12:08 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/01 13:17:15 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_value(const char *target, const char **mtx);
int		var_is_valid(const char *var);
int		var_check(const char *var);

/* just a var_is_valid() wrapper. 
takes both 'var' and 'var=' as valid string. */
int	var_check(const char *var)
{
	char	my_var[VARNAME];
	int		eq;
	
	eq = ft_strichr(var, '=');
	if (eq == 1 || eq > VARNAME)
		return (0);
	//ft_printf("no = found\n");
	ft_bzero(my_var, VARNAME);
	if (eq != 0)
		ft_strlcpy(my_var, var, eq + 1);
	else
		ft_strlcpy(my_var, var, ft_varlen(var) + 1);
	if (!var_is_valid(my_var))
		return (0);
	return (1);
}

/* checks if the var is a valid name:
	no spaces (neither leading nor trailing)
	only alphabetical chars (upper or lowe)
	only separator allowed: '_'
RETURNS: 1 if the var is valid, 0 if it isn't.
NOTE: you should throw a 'not a valid identifier' error (Exit Code: 1). */
int	var_is_valid(const char *var)
{
	int	i;

	if (var == NULL)
		return (0);
	//ft_printf("validating: '%s'\n", var);
	i = 0;
	while (var[i] != '\0')
	{
		if (i == 0 && !ft_isalpha(var[i]))
			return (0);
		if (!ft_isalpha(var[i]) && !ft_isdigit(var[i]) && var[i] == '_')
			return (0);
		i++;
	}
	return (1);
}

/*
var1=123
var2=laskd
var3=
var4=123;12k3
NOTE: target: 'var' without the '=' */
char	*get_value(const char *target, const char **mtx)
{
	size_t	tar_len;
	int		index;
	char	*my_tar;
	
	if (target == NULL || mtx == NULL)
		return (NULL);
	tar_len = ft_varlen(target);			//just for flexible usage
	my_tar = ft_substr(target, 0, tar_len); 	//VARLEN DISCUSSION
	//ft_printf("looking for '%s' -> '%s' in vars[?]\n", target, my_tar);
	if (!var_check(target))
	{
		//throw error? (treated as a cmd (or a path if contains '/'))
		free(my_tar);
		return (NULL);
	}
	index = is_there(mtx, my_tar);
	if (index >= 0)
	{
		//ft_printf("found '%s' in vars[?]\n", mtx[index]);
		free(my_tar);
		return(ft_substr(mtx[index], tar_len + 1, ft_strlen(mtx[index] + tar_len + 1)));
	}
	free(my_tar);
	return (NULL);
}
