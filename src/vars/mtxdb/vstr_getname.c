/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vstr_getname.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 17:41:31 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/15 14:40:17 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "varstr.h"

char	*vstr_getname(char *varstr, char *buff, size_t size);
int		vstr_name_is_valid(char *varstr);

/* checks if the var has a valid name:
	no spaces (neither leading nor trailing)
	only alphabetical chars (upper or lower)
	only separator allowed: '_'
RETURNS: 1 if the var is valid, 0 if it isn't, -1 on malloc failure.
NOTE: you should throw a 'not a valid identifier' error (Exit Code: 1). */
int	vstr_name_is_valid(char *varstr)
{
	size_t	i;
	char	*name;

	if (varstr == NULL)
		return (0);
	name = vstr_getname(varstr, NULL, 0);
	if (name == NULL)
		return (-1);
	if (name[0] == '\0')
		return (free(name), 0);
	i = 0;
	while (name[i] != '\0')
	{
		if (i == 0 && !(ft_isalpha(name[i]) || name[i] == '_'))
			return (free(name), 0);
		else if (!(ft_isalpha(name[i])
				|| ft_isdigit(name[i])
				|| name[i] == '_'))
			return (free(name), 0);
		i++;
	}
	free(name);
	return (1);
}

/* Copies the 'value' of the var-string into 'buff',
if buff isn't NULL. Otherwise strdups the value.
RETURNS: a pointer to a dedicated space containing the value,
NULL on malloc failure or if not a var-string. */
char	*vstr_getname(char *varstr, char *buff, size_t size)
{
	size_t	i;
	char	*name;

	if (buff != NULL)
		ft_memset(buff, 0, size);
	if (varstr == NULL)
		return (NULL);
	i = 0;
	while (varstr[i] && varstr[i] != '=')
		i++;
	if (buff != NULL)
	{
		if (size > (size_t)i)
			size = i + 1;
		ft_strlcpy(buff, varstr, size);
		return (buff);
	}
	name = ft_substr(varstr, 0, i);
	if (name == NULL)
		write(STDERR_FILENO, "vstr: malloc failure\n", 21);
	return (name);
}
