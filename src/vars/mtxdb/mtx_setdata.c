/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mtx_setdata.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:31:39 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/01 13:35:05 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mtx.h"

void	mtx_setdata(int value, char **mtx);

/* Takes a mtx-matrix and an integer as parameters.
Sets the second free slot after 'size' to the integer passed:
*(int *)mtx = size, *(int *)(mtx + 1) = value. */
void	mtx_setdata(int value, char **mtx)
{
	*(int *)(mtx + 1) = value;
}
