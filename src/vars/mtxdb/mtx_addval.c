/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mtx_addval.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 15:55:21 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/12 18:29:42 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mtx.h"

char	**mtx_addval(char *name, char *value, char **mtx);

static char	*build_var(char *name, char *value)
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
		write(STDERR_FILENO, "mtx: malloc failure\n", 20);
		return (NULL);
	}
	ft_strlcpy(var, name, ft_strlen(name) + 1);
	if (value == NULL)
		return (var);
	ft_strlcat(var, "=", ft_strlen(var) + 2);
	ft_strlcat(var, value, ft_strlen(var) + ft_strlen(value) + 1);
	return (var);
}

/* Assign a new var-string to the first mtx 'empty-slot':
mtx[n] = "name=vaue".
If there are no more empty spaces the matrix is expanded with mtx_expand,
and mtx_addval is called again.
RETURNS: a mtx-pointer to the matrix with the var-string added.
If there was enough spaces in the previous matrix returns 'mtx'. */
char	**mtx_addval(char *name, char *value, char **mtx)
{
	unsigned int		i;

	i = 1;
	while (i < *(unsigned int *)mtx && mtx[i] != NULL)
		i++;
	if (i == *(unsigned int *)mtx)
	{
		mtx = mtx_addval(name, value, mtx_expand(mtx));
		return (mtx);
	}
	mtx[i] = build_var(name, value);
	return (mtx);
}
