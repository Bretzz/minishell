/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mtx_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 15:55:47 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/12 18:31:54 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mtx.h"

char	**mtx_init(void);
char	**mtx_expand(char **mtx);

/* RETURNS: a mtx-pointer with 16 slots allocated and set to zero.
NULL on malloc(3) failure. */
char	**mtx_init(void)
{
	char	**mtx;

	mtx = (char **)malloc(16 * sizeof(char *));
	if (mtx == NULL)
	{
		write(STDERR_FILENO, "mtx: malloc failure\n", 20);
		return (NULL);
	}
	ft_memset(mtx, 0, 16 * sizeof(char *));
	*(unsigned int *)mtx += 16;
	return (mtx);
}

/* Takes a mtx-pointer as a parameter and reallocs a matrix with 16 more slots.
Then updates the size written in the first slot. */
char	**mtx_expand(char **mtx)
{
	char	**new_mtx;

	if (mtx == NULL)
		return (mtx_init());
	new_mtx = ft_realloc(mtx, (*(unsigned int *)mtx) * sizeof(char *),
			(*(unsigned int *)mtx + 16) * sizeof(char *));
	if (new_mtx == NULL)
	{
		write(STDERR_FILENO, "mtx: malloc failure\n", 20);
		return (mtx);
	}
	ft_memset(&new_mtx[*(unsigned int *)new_mtx], 0, 16 * sizeof(char *));
	*(int *)new_mtx += 16;
	return (new_mtx);
}
