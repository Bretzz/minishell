/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   the_matrix.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totommi <totommi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 17:45:32 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/19 02:22:12 by totommi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		is_there(const char **mtx, const char *target);
void	**drop_index(void **mtx, int index);

/* checks weather the target var is present or not.
RETURNS: the index of the var, -1 if not present or mtx/target is NULL. */
int	is_there(const char **mtx, const char *target)
{
	char	*my_tar;
	size_t	tar_len;
	int		cmp;
	int		i;

	if (mtx == NULL || target == NULL)
		return (-1);
	i = 0;
	while (target[i] && target[i] != '=')
		i++;
	// if (*target == '$')	//both $var and var=value behaviour should be impelented
		//code here
	my_tar = ft_substr(target, 0, i);
	tar_len = ft_strlen(my_tar);
	i = 0;
	while (mtx[i] != NULL)
	{
		cmp = ft_strncmp(mtx[i], my_tar, tar_len + 1);
		if (cmp == 0 || cmp == '=')
			return (free(my_tar), i);
		i++;
	}
	free(my_tar);
	return(-1);
}

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
