/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mtx_findval.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 17:22:02 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/14 15:30:35 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mtx.h"

char	*mtx_findval(char *name, char *buff, size_t size, char **mtx);

/* Searches mtx for the string with the 'name' passed,
if buff == NULL allocates a new string to return.
if not just copies the value inside the buffer.
(up to size -1 chars from the string found in the mtx)
RETURNS: a pointer to the string containing the wanted value,
NULL if malloc fails. */
char	*mtx_findval(char *name, char *buff, size_t size, char **mtx)
{
	int	index;

	if (buff != NULL)
		ft_memset(buff, 0, size);
	if (name == NULL || mtx == NULL)
		return (NULL);
	if (vstr_name_is_valid(name) <= 0)
	{
		return (NULL);
	}
	index = mtx_getindex(name, mtx);
	if (index < 0)
		return (NULL);
	return (vstr_getvalue(mtx[index], buff, size));
}
