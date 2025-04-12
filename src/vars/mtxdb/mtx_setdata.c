/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mtx_setdata.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:31:39 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/12 18:13:27 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mtx.h"

void	mtx_setdata(int value, char **mtx, int slot);

/* Takes a mtx-matrix and an integer as parameters.
Sets the second free slot after 'size' to the integer passed:
REMEMBER: *(unsigned int *)mtx = size!.
Also if you use mtx_setdata() with slot=0 and then with slot=1 or slot=2
the data will be corrupted. */
void	mtx_setdata(int value, char **mtx, int slot)
{
	if (slot == 0)
		*((unsigned int *)mtx + 1) = value;
	if (slot == 1)
		*((unsigned char *)mtx + 5) = value;
	if (slot == 2)
		*((unsigned char *)mtx + 6) = value;
}
