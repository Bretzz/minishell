/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mtx_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 15:55:47 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/26 16:55:30 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mtx.h"

char	**mtx_init();
char	**mtx_expand(char **mtx);

/* RETURNS: a mtx-pointer with 16 slots allocated and set to zero.
NULL on malloc(3) failure. */
char	**mtx_init()
{
	char	**mtx;

	mtx = (char **)malloc(16 * sizeof(char *));
	if (mtx == NULL)
		return (NULL);
	ft_memset(mtx, 0, 16 * sizeof(char *));
	*(int *)mtx += 16;
	return (mtx);
}

/* Takes a mtx-pointer as a parameter and reallocs a matrix with 16 more slots.
Then updates the size written in the first slot. */
char	**mtx_expand(char **mtx)
{
	char **new_mtx;

	if (mtx == NULL)
		return (mtx_init());
	new_mtx = ft_realloc(mtx, (*(int *)mtx) * sizeof(char *), (*(int *)mtx + 16) * sizeof(char *));
	if (new_mtx == NULL)
	{
		write(STDERR_FILENO, "malloc failure\n", 15);
		return (mtx);
	}
	ft_memset(&new_mtx[*(int *)new_mtx], 0, 16 * sizeof(char *));
	*(int *)new_mtx += 16;
	return (new_mtx);
}
