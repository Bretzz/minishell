/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mtx_addnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 18:07:04 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/06 16:20:01 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mtx.h"

char	**mtx_addnum(char *name, int value, char **mtx);

static char *build_var(char *name, int value)
{
	char	*var;
	char	*strval;
	size_t	len;
	
	if (name == NULL)
		return (NULL);
	strval = ft_itoa(value);
	if (strval == NULL)
	{
		write(STDERR_FILENO, "mtx: malloc failure\n", 20);
		return (NULL);
	}
	len = ft_strlen(name) + ft_strlen(strval) + 2;
	var = (char *)ft_calloc(len, sizeof(char));
	if (var == NULL)
	{
		write(STDERR_FILENO, "mtx: malloc failure\n", 20);
		return (NULL);
	}
	ft_strlcpy(var, name, ft_strlen(name) + 1);
	ft_strlcat(var, "=", ft_strlen(var) + 2);
	ft_strlcat(var, strval, ft_strlen(var) + ft_strlen(strval) + 1);
	free(strval);
	return (var);
}

/* same as setval but with an integer instead of as string as 'value' */
char	**mtx_addnum(char *name, int value, char **mtx)
{
	unsigned int		i;

	i = 1;
	while (i < *(unsigned int *)mtx && mtx[i] != NULL)
		i++;
	if (i == *(unsigned int *)mtx)
	{
		//ft_printf("reached mtx limit, proceeding with expansion\n");
		mtx = mtx_addnum(name, value, mtx_expand(mtx));
		return (mtx);
	}
	mtx[i] = build_var(name, value);
	return (mtx);
}
