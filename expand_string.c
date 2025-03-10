/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_string.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 19:25:47 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/10 21:25:02 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *expand_string(char *str, char **shv, char **env);

static char	*cut_string(char *str, char *target)
{
	char	*new_str;
	char	*tar_ptr;
	int		tar_len;
	int		i;

	if (str == NULL || target == NULL)
		return (free(target), str);
	tar_len = ft_strlen(target);
	tar_ptr = ft_strnstr(str, target, ft_strlen(str));
	if (tar_ptr == NULL)
		return (free(target), str);
	new_str = (char *)malloc(ft_strlen(str) - ft_strlen(target) + 1);
	if (new_str == NULL)
		return (free(target), NULL);
	i = 0;
	while (str[i] != '\0')
	{
		if (&str[i] != tar_ptr)
		{
			new_str[i] = str[i];
			i++;
		}
		else
			str += tar_len;
	}
	new_str[i] = '\0';
	free(target);
	return (new_str);
}

/* takes a string (to be free'd) and the index of the var found.
Allocates with malloc(3) a new string that's a copy of the imput string
with the var_string '$var' replaced with it's value found in shv or env,
then frees the input string.
The index 'i' is increased by the length of the varname (e.g. $var = 4).
RETURNS: A pointer to the new string, NULL on malloc failures. */
static char	*single_expand(int *i, char *str, char **shv, char **env)
{
	char	*exp_val;
	char	*new_str;
	int		var_len;

	exp_val = get_value(&str[*i + 1], (const char **)shv, (const char **)env);
	if (exp_val == NULL)
	{
		new_str = cut_string(str, ft_substr(str, *i, ft_strlen_space(&str[*i])));
		*i += ft_strlen_space(&str[*i]) - 1;
		return (new_str);
	}
	var_len = ft_strlen_space(&str[*i]);
	new_str = (char *)malloc(ft_strlen(str) - var_len + ft_strlen(exp_val));
	if (new_str == NULL)
		return (free(exp_val), NULL);
	ft_strlcpy(new_str, str, ft_strichr(str, '$'));
	ft_strlcat(new_str, exp_val, ft_strlen(new_str) + ft_strlen(exp_val));
	ft_strlcat(new_str, &str[*i + var_len], ft_strlen(str) + ft_strlen(&str[*i + var_len]));
	*i += var_len - 1;
	free(str);
	return (new_str);
}

/* takes a string, the shvars and the env vars as parameter.
returns a mallocated string with the variables preceded by '$' expanded.
RETURNS: the expanded string, NULL on malloc error. */
char *expand_string(char *str, char **shv, char **env)
{
	char	*my_str;
	int		i;

	my_str = ft_strdup(str);
	if (my_str == NULL)
		return (NULL);	
	i = 0;
	while (my_str[i] != '\0')
	{
		if (my_str[i] == '$')
			my_str = single_expand(&i, my_str, shv, env);
		i++;
	}
	return (my_str);
}
