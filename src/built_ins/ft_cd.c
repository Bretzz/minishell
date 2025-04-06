/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 20:29:28 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/06 14:06:27 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_cd(int *fd, t_cmd cmd, char ***vars);

static void	update_pwd(char *oldpwd, char *pwd, char ***vars)
{
	static int	fresh;
	int			index;
	// char		*env_oldpwd;

	if (pwd != NULL)
	{
		index = mtx_getindex("PWD", vars[1]);
		if (index >= 0)
			mtx_setval("PWD", pwd, vars[1]);
		// else
		// {
		// 	index = mtx_getindex("OLDPWD", vars[1]);
		// 	if (index >= 0)
		// 	{
		// 		env_oldpwd = mtx_findval("OLDPWD", NULL, 0, vars[1]);
		// 		if (env_oldpwd == NULL)
		// 			fresh++;
		// 		else if (!ft_strncmp(env_oldpwd, pwd, ft_strlen(oldpwd) + 1))
		// 			*(ft_strchr(vars[1][index], '=')) = '\0';
		// 		free(env_oldpwd);
		// 	}
		// }
	}
	if (oldpwd != NULL)
	{
		index = mtx_getindex("OLDPWD", vars[1]);
		if (index >= 0 && (ft_strchr(vars[1][index], '=') != NULL || fresh != 0))
		{
			mtx_setval("OLDPWD", oldpwd, vars[1]);
			fresh = 0;
		}
	}
}

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

	! ! ! NOTE ! ! !
	If,  during  the  execution of the above steps, the PWD environment variable is
	set, the OLDPWD environment variable shall also be set to the value of the  old
	working  directory  (that is the current working directory immediately prior to
	the call to cd).

RETURNS: 1 on error, 0 on successfull execution. */
int	ft_cd(int *fd, t_cmd cmd, char ***vars)
{
	char	*oldpwd;
	char	*pwd;
	char	*tar_dir;

	safeclose(fd[1]);
	if (cmd.words[2] != NULL)
	{
		write(STDERR_FILENO, "minishell: cd: too many argument\n", 33);
		return (1);
		//better error handling
	}
	if (cmd.words[1] == NULL)
	{
		tar_dir = mtx_findval("HOME", NULL, 0, vars[1]);
		if (tar_dir == NULL)
		{
			write(STDERR_FILENO, "minishell: cd: HOME not set\n", 28);
			return (1);
		}
	}
	else
		tar_dir = cmd.words[1];
	//ft_printf("new-dir=%s\n", cmd.words[1]);
	oldpwd = getcwd(NULL, 0);
	if (chdir(tar_dir) < 0)
	{
		//handle errno
		return (1); //return errno
	}
	pwd = getcwd(NULL, 0);	//catch error
	update_pwd(oldpwd, pwd, vars);
	free(oldpwd); free(pwd);
	if (cmd.words[1] == NULL)
		free(tar_dir);
	//ft_printf("  TODO: update PWD automatically\n\tgo HOME if only 'cd'\n\tjust read the man...\n");
	return (0);
}
