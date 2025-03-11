/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   the_matrix.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 17:45:32 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/11 18:11:05 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* takes a matrix and an index as parameters.
Reallocs a new matrix with same elements but the one at the index.
RETURN: the new matrix, NULL on malloc(3) failure, the same matrix on IOR. 
(IOR: Index Out of Range)
NOTE: the areas pointed by the elements of the matrix is left untouched,
except for the one at the index witch is free'd. */
void	**drop_index(void **mtx, int index)
{
	void	**new_mtx;
	size_t	len;
	int		i;

	len = ft_mtxlen((const void **)mtx);
	if (index < 0 || index >= (int)len)
		return (mtx);
	new_mtx = (void **)malloc((len) * sizeof(void *));
	if (new_mtx == NULL)
		return (ft_printfd(2, "malloc failure\n"), mtx);
	i = 0;
	while (mtx[i] != NULL)
	{
		if (i == index)
		{
			free(mtx[i]);
			mtx++;
		}
		if (mtx[i] == NULL)
			break ;
		new_mtx[i] = mtx[i];
		i++;
	}
	new_mtx[i] = NULL;
	return (free(--mtx), new_mtx);
}
