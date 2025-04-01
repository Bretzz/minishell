/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mtx_setnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 19:21:32 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/26 19:39:54 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mtx.h"

char	**mtx_setnum(char *name, int value, char **mtx);

/* Safer alternative to mtx_addnum(), if a var with 'name' is present,
uses mtx_replace(), otherwise uses mtx_addnum */
char	**mtx_setnum(char *name, int value, char **mtx)
{
	char	*strval;
	int		index;

	if (name == NULL)
		return (mtx);
	strval = ft_itoa(value);
	if (strval == NULL)
	{
		write(STDERR_FILENO, "malloc failure\n", 15);
		return (mtx);
	}
	index = mtx_getindex(name, mtx);
	if (index < 0)
	{
		mtx = mtx_addval(name, strval, mtx);
		return (free(strval), mtx);
	}
	mtx_replace(name, strval, mtx);
	return (free(strval), mtx);
}
