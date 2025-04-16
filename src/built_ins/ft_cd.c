/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 20:29:28 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/16 21:59:26 by topiana-         ###   ########.fr       */
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
		mtx_setval("OLDPWD", oldpwd, vars[1]);
}

static int	env_is_valid(char *varenv, char ***vars)
{
	char	temp_var[MAX_PATH + 10];
	char	*really_temp_var;

	really_temp_var = NULL;
	if (!mtx_findval(varenv, temp_var, sizeof(temp_var), vars[1]))
	{
		ft_printfd(STDERR_FILENO, "minishell: cd: %s not set\n", varenv);
		return (0);
	}
	if (ft_strlen(temp_var) > MAX_PATH)
	{
		really_temp_var = mtx_findval(varenv, NULL, 0, vars[1]);
		if (really_temp_var == NULL)
			ft_perror(varenv, "File name too long", NULL, 0);
		else
			ft_perror(really_temp_var, "File name too long", NULL, 0);
		return (free(really_temp_var), 0);
	}
	return (1);
}

static int	arg_path_is_valid(char *arg)
{
	if (ft_strlen(arg) > MAX_PATH)
	{
		ft_perror(arg, "File name too long", NULL, 0);
		return (0);
	}
	return (1);
}

// helper function
static int	get_tar_dir(char *tar_dir, t_cmd cmd, char ***vars)
{
	if (cmd.words[2] != NULL)
	{
		write(STDERR_FILENO, "minishell: cd: too many argument\n", 33);
		return (EXIT_FAILURE);
	}
	if (cmd.words[1] == NULL)
	{
		if (!env_is_valid("HOME", vars))
			return (EXIT_FAILURE);
		mtx_findval("HOME", tar_dir, MAX_PATH + 1, vars[1]);
	}
	else if (!ft_strncmp(cmd.words[1], "-", 2))
	{
		if (!env_is_valid("OLDPWD", vars))
			return (EXIT_FAILURE);
		mtx_findval("OLDPWD", tar_dir, MAX_PATH + 1, vars[1]);
		ft_printfd(STDOUT_FILENO, "%s\n", tar_dir);
	}
	else
	{
		if (!arg_path_is_valid(cmd.words[1]))
			return (EXIT_FAILURE);
		ft_strlcpy(tar_dir, cmd.words[1], MAX_PATH + 1);
	}
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
	char	tar_dir[MAX_PATH + 1];

	safeclose(fd[1]);
	ft_memset(tar_dir, 0, MAX_PATH + 1);
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
