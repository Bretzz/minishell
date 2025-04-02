/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 20:29:28 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/02 11:03:50 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_cd(int *fd, t_cmd cmd, char ***vars);

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
RETURNS: 1 on error, 0 on successfull execution. */
int	ft_cd(int *fd, t_cmd cmd, char ***vars)
{
	int		ret;
	char	*oldpwd;

	multicose(fd);
	if (cmd.words[2] != NULL)
	{
		ft_printfd(STDERR_FILENO, "minishell: cd: too many argument\n");
		return (1);
		//better error handling
	}
	if (cmd.redir[0] == PIPE || cmd.redir[1] == PIPE) //maybe execute in child if in pipe
		return (1);
	//ft_printf("new-dir=%s\n", cmd.words[1]);
	oldpwd = getcwd(NULL, 0);
	ret = chdir(cmd.words[1]);
	if (ret == -1)
	{
		//handle errno
		return (1); //return errno
	}
	vars[1] = mtx_setval("OLDPWD", oldpwd, vars[1]);
	vars[2] = mtx_setval("OLDPWD", oldpwd, vars[2]);
	free(oldpwd);
	ft_printf("  TODO: update PWD automatically\n\tgo HOME if only 'cd'\n\tjust read the man...\n");
	return (0);
}
