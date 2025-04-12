/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vstr_getvalue.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 17:30:21 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/12 18:33:33 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "varstr.h"

char	*vstr_getvalue(char *varstr, char *buff, size_t size);

/* Copies the 'value' of the var-string into 'buff',
if buff isn't NULL. Otherwise strdups the value.
RETURNS: a pointer to a dedicated space containing the value,
NULL on malloc failure or if not a var-string.
NOTE: alloc mode is preferable if you are dealing with var-strings
with no value, e.g. "VARNAME" (not "VARNAME="). In buffer-mode
vstr_getvalue() retusns a bzero'd buffer in both cases,
in alloc mode the first case returns a NULL pointer, in the second
case retusn an allocated space of size=1 cointaining a '\0'. */
char	*vstr_getvalue(char *varstr, char *buff, size_t size)
{
	size_t	i;
	char	*value;

	if (buff != NULL)
		ft_memset(buff, 0, size);
	if (varstr == NULL)
		return (NULL);
	i = 0;
	while (varstr[i] && varstr[i] != '=')
		i++;
	if (varstr[i] == '\0')
	{
		return (NULL);
	}
	if (buff != NULL)
	{
		ft_strlcpy(buff, &varstr[i + 1], size);
		return (buff);
	}
	value = ft_strdup(&varstr[i + 1]);
	if (value == NULL)
		write(STDERR_FILENO, "vstr: malloc failure\n", 21);
	return (value);
}
