/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 20:29:28 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/18 19:42:14 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_cd(int *fd, t_cmd cmd);

/*	EACCES Search permission is denied for any component of the pathname.

	ELOOP  A loop exists in symbolic links encountered  during  resolution  of  the
			path argument.

	ENAMETOOLONG
			The length of a component of a pathname is longer than {NAME_MAX}.

	ENOENT A  component  of  path does not name an existing directory or path is an
			empty string.

	ENOTDIR
			A component of the pathname names an existing file that is neither a di‐
			rectory nor a symbolic link to a directory.

	The chdir() function may fail if:

	ELOOP  More  than  {SYMLOOP_MAX} symbolic links were encountered during resolu‐
			tion of the path argument.

	ENAMETOOLONG
			The length of a pathname exceeds {PATH_MAX}, or pathname resolution of a
			symbolic link produced an intermediate result with a length that exceeds
			{PATH_MAX}.
RETURNS: -1 on error, 1 on successfull execution. */
int	ft_cd(int *fd, t_cmd cmd)
{
	int		ret;

	if (cmd.words[2] && cmd.words[2][0] != '\0' )
	{
		ft_printfd(STDERR_FILENO, "minishell: cd: Too many argument\n");
		multicose(fd);
		return (1);
		//better error handling
	}
	ft_printf("new-dir=%s\n", cmd.words[1]);
	ret = chdir(cmd.words[1]);
	if (ret == -1)
	{
		//handle errno
		return (1); //return errno
	}
	return (0);
}
