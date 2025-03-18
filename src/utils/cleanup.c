/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 19:27:18 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/18 20:18:17 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
