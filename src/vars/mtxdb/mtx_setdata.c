/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mtx_setdata.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:31:39 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/02 10:45:50 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mtx.h"

void	mtx_setdata(int value, char **mtx);

/* Takes a mtx-matrix and an integer as parameters.
Sets the second free slot after 'size' to the integer passed:
*(unsigned int *)mtx = size, *(unsigned int *)(mtx + 1) = value. */
void	mtx_setdata(int value, char **mtx)
{
	*((unsigned int *)mtx + 1) = value;
	// ft_printf("new matrix:\n[0] (%p) [%i, %i]\n[1] (%p) [%s]\n",
	// 	mtx[0], *(unsigned int *)(mtx),
	// 	*((unsigned int *)mtx + 1),
	// 	mtx[1],
	// 	mtx[1]);
}
