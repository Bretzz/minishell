/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mtx_setval.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 19:16:08 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/26 19:26:17 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mtx.h"

char	**mtx_setval(char *name, char *value, char **mtx);

/* Safer alternative to mtx_addval(), if a var with 'name' is present,
uses mtx_replace(), otherwise uses mtx_addval */
char	**mtx_setval(char *name, char *value, char **mtx)
{
	int	index;

	if (name == NULL)
		return (mtx);
	index = mtx_getindex(name, mtx);
	if (index < 0)
		return (mtx_addval(name, value, mtx));
	mtx_replace(name, value, mtx);
	return (mtx);
}
