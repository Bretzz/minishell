/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_safe_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 19:42:07 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/15 19:46:51 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* get_next_line that will resume if a
SIGQUIT or SIGTSPT is sent.
REMEMBER: to use the signale initializer
before calling get_safe_line(). */
char	*get_safe_line(int fd)
{
	char	*line;

	line = get_next_line(fd);
	while (!line && (g_last_sig == SIGQUIT || g_last_sig == SIGTSTP))
		line = get_next_line(fd);
	return (line);
}
