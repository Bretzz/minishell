/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totommi <totommi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 19:27:18 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/10 14:29:33 by totommi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	safeclose(int fd);
void	multicose(int *fd);
void	free_mtx(void **mtx);
void	free_mtx_mtx(void ***mtx);

void	safeclose(int fd)
{
	if (fd > 2)
		close(fd);
}

/* expected a pointer to a space with at least 2 integers.
checks's weather the value is bigger than 2,
if it is close(3)s the corresponding file descriptor. */
void	multicose(int *fd)
{
	safeclose(fd[0]);
	safeclose(fd[1]);
}

void	free_mtx(void **mtx)
{
	unsigned int	i;
	
	if (mtx == NULL)
	return ;
	i = 0;
	while (mtx[i] != NULL)
	{
		free(mtx[i]);
		i++;
	}
	free(mtx);
}

void	free_mtx_mtx(void ***mtx)
{
	unsigned int	i;

	if (mtx == NULL)
		return ;
	i = 0;
	while (mtx[i] != NULL)
	{
		free_mtx(mtx[i]);
		i++;
	}
	free(mtx);
}
