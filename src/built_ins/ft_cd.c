/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 20:29:28 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/11 14:11:22 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "built_ins.h"

int	ft_cd(int *fd, t_cmd cmd, char ***vars);

static void	update_pwd(char *oldpwd, char *pwd, char ***vars)
{
	int			index;

	if (pwd != NULL)
	{
		index = mtx_getindex("PWD", vars[1]);
		if (index >= 0)
			mtx_setval("PWD", pwd, vars[1]);
		else
			return ;
	}
	if (oldpwd != NULL)
	{
		index = mtx_getindex("OLDPWD", vars[1]);
		if (index >= 0)
		{
			mtx_setval("OLDPWD", oldpwd, vars[1]);
		}
	}
}

// helper function
int	get_tar_dir(char *tar_dir, t_cmd cmd, char ***vars)
{
	if (cmd.words[2] != NULL)
	{
		write(STDERR_FILENO, "minishell: cd: too many argument\n", 33);
		return (EXIT_FAILURE);
	}
	if (cmd.words[1] == NULL)
	{
		if (!mtx_findval("HOME", tar_dir, MAX_PATH, vars[1]))
		{
			write(STDERR_FILENO, "minishell: cd: HOME not set\n", 28);
			return (EXIT_FAILURE);
		}
	}
	else
		ft_strlcpy(tar_dir, cmd.words[1], MAX_PATH);
	return (EXIT_SUCCESS);
}
/*	EACCES Search permission is denied for any component of the pathname.

	ELOOP  A loop exists in symbolic links encountered  during
	 resolution  of  the path argument.

	ENAMETOOLONG
			The length of a component of a pathname is longer than {NAME_MAX}.

	ENOENT A  component  of  path does not name an existing directory or path
	is an	empty string.

	ENOTDIR
			A component of the pathname names an existing file that is neither
			a directory nor a symbolic link to a directory.

	The chdir() function may fail if:

	ELOOP  More  than  {SYMLOOP_MAX} symbolic links were encountered 
	during resolution of the path argument.

	ENAMETOOLONG
			The length of a pathname exceeds {PATH_MAX}, or pathname
			resolution of a	symbolic link produced an intermediate result
			with a length that exceeds {PATH_MAX}.

	! ! ! NOTE ! ! !
	If,  during  the  execution of the above steps, the PWD environment
	variable is	set, the OLDPWD environment variable shall also be set to 
	the value of the  old working  directory  (that is the current working
	directory immediately prior to	the call to cd).

RETURNS: 1 on error, 0 on successfull execution. */

int	ft_cd(int *fd, t_cmd cmd, char ***vars)
{
	char	*oldpwd;
	char	*pwd;
	char	tar_dir[MAX_PATH];

	safeclose(fd[1]);
	if (get_tar_dir(tar_dir, cmd, vars) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	oldpwd = getcwd(NULL, 0);
	if (chdir(tar_dir) < 0)
	{
		ft_printfd(STDERR_FILENO, "minishell: cd: %s: %s\n", tar_dir,
			strerror(errno));
		free(oldpwd);
		return (errno);
	}
	pwd = getcwd(NULL, 0);
	update_pwd(oldpwd, pwd, vars);
	return (free(oldpwd), free(pwd), EXIT_SUCCESS);
}
