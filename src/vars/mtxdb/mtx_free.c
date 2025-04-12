/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mtx_free.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 16:13:50 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/12 18:30:18 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mtx.h"

void	mtx_free(char **mtx);

/* Default free mtx-pointer */
void	mtx_free(char **mtx)
{
	unsigned int	i;

	if (mtx == NULL)
		return ;
	i = 1;
	while (i < *(unsigned int *)mtx)
	{
		free(mtx[i]);
		i++;
	}
	free(mtx);
}
