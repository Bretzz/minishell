/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nest_putztr_fd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:40:40 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/19 12:45:38 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* prints a string up to len characters. */
int	nest_putztr_fd(int fd, char *str, int len)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while(str[i] && i < len)
	{
		count += write(fd, &str[i], 1);
		i++;
	}
	return (count);
}
