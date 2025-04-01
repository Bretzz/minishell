/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mtx_free.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 16:13:50 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/01 14:16:54 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mtx.h"

void	mtx_free(char **mtx);

/* Default free mtx-pointer */
void	mtx_free(char **mtx)
{
	int	i;

	ft_printf("freeing mtx: %p\n", mtx);
	if (mtx == NULL)
		return ;
	ft_printf("freeing mtx of size: %d\n", *(int *)mtx);
	i = 1;
	while (i < *(int *)mtx)
	{
		ft_printf("freeing: '%s'\n", mtx[i]);
		free(mtx[i]);
		i++;
	}
	free(mtx);
}
