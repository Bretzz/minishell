/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mtx_delvar.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 18:27:01 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/26 21:53:06 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mtx.h"

void	mtx_delvar(int index, char **mtx);
void	mtx_safedel(int index, char **mtx);

void	mtx_delvar(int index, char **mtx)
{
	free(mtx[index]);
	mtx[index] = NULL;	
}

/* Delete the index passed and pushes everything up */
void	mtx_safedel(int index, char **mtx)
{
	if (mtx == NULL)
		return ;
	mtx_delvar(index, mtx);
	mtx_pushup(mtx);
}
