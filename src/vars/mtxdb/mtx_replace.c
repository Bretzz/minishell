/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mtx_replace.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 18:49:04 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/26 21:53:11 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mtx.h"

void	mtx_replace(char *name, char *value, char **mtx);

static char *build_var(char *name, char *value)
{
	char	*var;
	size_t	len;
	
	if (name == NULL)
		return (NULL);
	len = ft_strlen(name) + 1;
	if (value != NULL)
		len += ft_strlen(value) + 1;
	var = (char *)ft_calloc(len, sizeof(char));
	if (var == NULL)
	{
		write(STDERR_FILENO, "malloc failure\n", 15);
		return (NULL);
	}
	ft_strlcpy(var, name, ft_strlen(name) + 1);
	if (value == NULL)
		return (var);
	ft_strlcat(var, "=", ft_strlen(var) + 2);
	ft_strlcat(var, value, ft_strlen(var) + ft_strlen(value) + 1);
	return (var);
}

/* Looks for a str-var with 'name' in 'mtx'. If found replaces
it with a new one with same name, and value passed. */
void	mtx_replace(char *name, char *value, char **mtx)
{
	int		index;

	if (name == NULL)
		return ;
	index = mtx_getindex(name, mtx);
	if (index < 0)
		return ;
	mtx_delvar(index, mtx);
	mtx[index] = build_var(name, value);
}
