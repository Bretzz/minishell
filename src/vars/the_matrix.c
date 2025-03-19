/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   the_matrix.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 17:45:32 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/19 18:30:01 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		is_there(const char **mtx, const char *target);
void	**drop_index(void **mtx, int index);

/* checks weather the target var is present or not.
RETURNS: the index of the var, -1 if not present or mtx/target is NULL.
EXPECTED: string 'varname' or 'varname='*/
int	is_there(const char **mtx, const char *target)
{
	char	*my_tar;
	size_t	tar_len;
	int		cmp;
	int		i;

	if (!mtx || !target)
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
	if (len == 1 && index == 0)
		return (free(mtx[0]), free(mtx), NULL);	//better syntex pls
	new_mtx = (void **)ft_calloc((len), sizeof(void *));
	if (new_mtx == NULL)
		return (ft_printfd(STDERR_FILENO, "malloc failure\n"), mtx);
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
	//ft_print_charr((const char **)new_mtx);
	return (free(--mtx), new_mtx);
}
