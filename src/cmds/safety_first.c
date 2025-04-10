/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safety_first.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: march <march@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 00:03:12 by march             #+#    #+#             */
/*   Updated: 2025/04/11 00:09:14 by march            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* returns a line read from the 'fd'.
If a SIGQUIT or a SIGTSTP is intercepted tries again. */
char	*safe_line(int fd)
{
	char	*line;

	line = get_next_line(fd);
	while (!line && (g_last_sig == SIGQUIT || g_last_sig == SIGTSTP))
	{
		g_last_sig = 0;
		line = get_next_line(fd);
	}
	return (line);
}

static void	clean_exit(int fd)
{
	char	path[MAX_PATH];

	safeclose(fd);
	unlink(get_doc_path(open_doc(GETNUM), path, MAX_PATH));
}
