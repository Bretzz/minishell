/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 19:42:26 by topiana-          #+#    #+#             */
/*   Updated: 2025/02/26 20:34:35 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_echo(char *cmd);

//echo     -n     ahahaha
int	ft_echo(char *cmd)
{
	int	i;

	i = 4;
	while (cmd[i] != '\0' && cmd[i] == ' ')
		i++;
	if (cmd[i] == '\0')
		return (0);
	if (cmd[i] == '-' && cmd[i + 1] == 'n')
	{
		i += 2;
		while (cmd[i] != '\0' && cmd[i] == ' ')
			i++;
		if (cmd[i] == '\0')
			return (0);
		return (ft_printf("%s", &cmd[i]));
	}
	return (ft_printf("%s\n", &cmd[i]));
}
