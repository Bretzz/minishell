/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_value.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 20:12:08 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/11 17:34:58 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_value(const char *target, const char **shv, const char **env);

/*
var1=123
var2=laskd
var3=
var4=123;12k3
NOTE: target: 'var' without the '='*/
char	*get_value(const char *target, const char **shv, const char **env)
{
	size_t	tar_len;
	char	*my_tar;
	int		index;
	
	if (target == NULL || (!shv && !env))
		return (NULL);
	tar_len = ft_strlen_space(target);
	my_tar = ft_substr(target, 0, tar_len);
	index = is_there(env, my_tar);
	if (index >= 0)
	{
		free(my_tar);
		return(ft_substr(env[index], tar_len + 1, ft_strlen(env[index] + tar_len + 1)));
	}
	index = is_there(shv, my_tar);
	if (index >= 0)
	{
		free(my_tar);
		return(ft_substr(shv[index], tar_len + 1, ft_strlen(shv[index] + tar_len + 1)));
	}
	free(my_tar);
	return (NULL);
}
