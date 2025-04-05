/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mtx_setdata.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totommi <totommi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:31:39 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/05 18:14:36 by totommi          ###   ########.fr       */
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
}
