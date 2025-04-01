/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 20:13:06 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/01 16:37:34 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char *))
{
	unsigned int	i;

	if (s == NULL)
		return ;
	i = 0;
	while (s[i] != '\0')
	{
		(*f)(i, &s[i]);
		i++;
	}
}
/*
#include <stdio.h>

void	to_num(unsigned int i, char *c)
{
	*c = i + '0';
}

int	main(int argc, char *argv[])
{
	ft_striteri(argv[1], &to_num);
	printf("%s\n", argv[1]);
	return (0);
}*/
