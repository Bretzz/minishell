/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mtx_vstr_copy.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 22:21:31 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/01 13:52:13 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mtx.h"

char	**mtx_vstr_copy(char *varstr, char **mtx);

char	**mtx_vstr_copy(char *varstr, char **mtx)
{
	char	*name;
	char	*value;
	
	if (varstr == NULL || mtx == NULL)
		return (mtx);
	name = vstr_getname(varstr, NULL, 0);
	value = vstr_getvalue(varstr, NULL, 0);
	if (!name)
		return (mtx);
	mtx = mtx_setval(name, value, mtx);
	free(name);
	free(value);
	return (mtx);
}
