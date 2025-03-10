/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_value.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 20:12:08 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/10 21:02:30 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_value(const char *target, const char **shv, const char **env);

static char	*check_shv(int *index, int tar_len, char *target, const char **shv)
{
	int	var_len;
	
//	ft_print_charr(shv);
//	ft_printf("check_shv: i[0]=%i, i[1]=%i, i[2]=%i\n", index[0], index[1], index[2]);
	if (shv && index[1] && shv[index[0]] != NULL
		&& !ft_strncmp(shv[index[0]], target, tar_len + 1))
	{
		free(target);
		var_len = ft_strlen(shv[index[0]]) - (tar_len + 1);
		return (ft_substr(shv[index[0]], tar_len + 1, var_len));
	}
	else if (shv && shv[index[0]] == NULL)
		index[1] = 0;
	return (NULL);
}

static char	*check_env(int *index, int tar_len, char *target, const char **env)
{
	int	var_len;
	
	if (env && index[2] && env[index[0]] != NULL
		&& !ft_strncmp(env[index[0]], target, tar_len + 1))
	{
		free(target);
		var_len = ft_strlen(env[index[0]]) - (tar_len + 1);
		return (ft_substr(env[index[0]], tar_len + 1, var_len));
	}
	else if (env && env[index[0]] == NULL)
		index[2] = 0;
	return (NULL);
}

/*
var1=123
var2=laskd
var3=
var4=123;12k3
*/
char	*get_value(const char *target, const char **shv, const char **env)
{
	size_t	tar_len;
	char	*my_tar;
	char	*value;
	int		i[3];
	
	if (target == NULL || (!shv && !env))
		return (NULL);
	tar_len = ft_strlen_space(target);
	my_tar = ft_substr(target, 0, tar_len);
	my_tar[tar_len] = '=';
	i[0] = 0;
	i[1] = 1;
	i[2] = 1;
	while ((shv && i[1]) || (env && i[2]))
	{
		value = check_shv(i, tar_len, my_tar, shv);
		if (value != NULL)
			return (value);
		value = check_env(i, tar_len, my_tar, env);
		if (value != NULL)
			return (value);
		i[0]++;
	}
	free(my_tar);
	return (NULL);
}
