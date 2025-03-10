/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_stuff.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 14:21:42 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/10 17:47:11 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_print_charr(const char **arr)
{
	int	i;

	if (arr == NULL)
	{
		ft_printf("%p\n", arr);
		return ;
	}
	i = 0;
	while (arr[i] != NULL)
	{
		ft_printf("%p='%s'\n", arr[i], arr[i]);
		i++;
	}
}
