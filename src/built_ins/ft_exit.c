/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 18:15:13 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/14 19:06:41 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_exit(int *fd, t_cmd cmd, char ***vars);

int	ft_exit(int *fd, t_cmd cmd, char ***vars)
{
	int exit_code;
	
	safeclose(fd[1]);
	exit_code = *((unsigned char *)vars[0] + 5);
	if (cmd.words[1] != NULL)
	{
		if (!is_number(cmd.words[1]))
		{
			ft_printfd(STDERR_FILENO, "minishell: \
exit: %s: numeric argument required\n", cmd.words[1]);
			exit_code = 2;
		}
		else
			exit_code = ft_atoi(cmd.words[1]);	
	}
	return (exit_code);
}
