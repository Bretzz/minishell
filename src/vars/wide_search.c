/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wide_search.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 22:21:00 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/12 22:21:19 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*wide_search(char *str, const char ***vars);

/* searches all the vars arrays for the var pointer by str */
char	*wide_search(char *str, const char ***vars)
{
	char	*value;
	char	*name;
	int		i;

	name = ft_substr(str, 0, ft_varlen(str));
	if (!name)
		return (NULL);
	i = 1;
	while (i >= 0)
	{
		value = mtx_findval(name, NULL, 0, (char **)(vars[i]));
		if (value != NULL)
		{
			free(name);
			return (value);
		}
		i--;
	}
	free(name);
	return (NULL);
}
