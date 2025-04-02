/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mtx_free.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 16:13:50 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/02 10:46:29 by topiana-         ###   ########.fr       */
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
		// if (mtx[i] != NULL)
		// 	ft_printf("freeing: (%p) '%s'\n", mtx[i], mtx[i]);
		// else
		// 	ft_printf("%p\n", mtx[i]);
		free(mtx[i]);
		i++;
	}
	free(mtx);
}
