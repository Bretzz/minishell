/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mtx_getindex.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 18:50:19 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/06 19:45:18 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mtx.h"

int	mtx_getindex(char *name, char **mtx);

/* RETURNS: the index of the var with 'name', -1 if name is NULL
of the var isn't in mtx. */
int	mtx_getindex(char *name, char **mtx)
{
	unsigned int	i;
	size_t			len;
	char			buff[MAX_NAME];

	if (name == NULL)
		return (-1);
	len = ft_strlen(name) + 1;
	i = 1;
	while (i < *(unsigned int *)mtx)
	{
		if (mtx[i] != NULL
			&& !ft_strncmp(name, vstr_getname(mtx[i], buff, sizeof(buff)), len))
			return (i);
		i++;
	}
	return (-1);
}
