/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mtx_move.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 21:28:55 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/12 18:32:15 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mtx.h"

void	mtx_move(int src, int dest, char **mtx);
void	mtx_pushup(char **mtx);

void	mtx_move(int src, int dest, char **mtx)
{
	if (mtx == NULL)
		return ;
	if (src < 1 || (unsigned int)src >= *(unsigned int *)mtx
		|| dest < 1 || (unsigned int)dest >= *(unsigned int *)mtx)
		return ;
	mtx[dest] = mtx[src];
	mtx[src] = NULL;
}

void	mtx_pushup(char **mtx)
{
	unsigned int	i[2];

	if (mtx == NULL)
		return ;
	ft_memset(i, 0, 2 * sizeof(int));
	while (i[0] < *(unsigned int *)mtx)
	{
		if (mtx[i[0]] == NULL)
		{
			i[1] = i[0];
			while (i[1] < *(unsigned int *)mtx && mtx[i[1]] == NULL)
				i[1]++;
			if (i[1] < *(unsigned int *)mtx)
				mtx_move(i[1], i[0], mtx);
		}
		i[0]++;
	}
}
