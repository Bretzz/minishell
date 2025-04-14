/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mtx_getindex.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 18:50:19 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/14 14:54:20 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mtx.h"

int	mtx_getindex(char *name, char **mtx);

/* RETURNS: the index of the var with 'name',
-1 if name is NULL, if the var isn't in mtx
or in case of malloc failure. */
int	mtx_getindex(char *name, char **mtx)
{
	unsigned int	i;
	size_t			len;
	char			*buff;

	if (name == NULL)
		return (-1);
	len = ft_strlen(name) + 1;
	i = 1;
	while (i < *(unsigned int *)mtx)
	{
		if (mtx[i] != NULL)
		{
			buff = vstr_getname(mtx[i], NULL, 0);
			if (buff == NULL)
				return (-1);
			if (!ft_strncmp(name, buff, len))
				return (free(buff), i);
			free(buff);
		}
		i++;
	}
	return (-1);
}
