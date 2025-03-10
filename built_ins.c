/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 19:42:26 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/10 19:44:11 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_echo(char *cmd, char **shv, char **env);
int	ft_cd(char *cmd);
int	ft_pwd(char *cmd);

//echo     -n     ahahaha
int	ft_echo(char *cmd, char **shv, char **env)
{
	char	*exp_str;
	int		i;

	i = 4;
	while (cmd[i] != '\0' && cmd[i] == ' ')
		i++;
	if (cmd[i] == '\0')
		return (-1);
	if (cmd[i] == '-' && cmd[i + 1] == 'n' && cmd[i + 2] == ' ')
	{
		i += 3;
		while (cmd[i] != '\0' && cmd[i] == ' ')
			i++;
		if (cmd[i] == '\0')
			return (-1);
		exp_str = expand_string(&cmd[i], shv, env);
		ft_printf("%s", exp_str);
		free(exp_str);
		return (1);
	}
	exp_str = expand_string(&cmd[i], shv, env);
	ft_printf("%s\n", exp_str);
	free(exp_str);
	return (1);
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
RETURNS: -1 on error, 1 on successfull execution. */
int	ft_cd(char *cmd)
{
	int		ret;
	char	*path;

	path = ft_strchr(cmd, ' ');
	if (path == NULL)
		return (1);
	path++;
	ft_printf("new-dir=%s\n", path);
	ret = chdir(path);
	if (ret == -1)
	{
		//handle errno
		return (-1);
	}
	return (1);
}

/*	EACCES Permission to read or search a component of the filename was denied.

	EFAULT buf points to a bad address.

	EINVAL The size argument is zero and buf is not a null pointer.

	ENAMETOOLONG
			getwd(): The size of the null-terminated absolute  pathname  string  ex‐
			ceeds PATH_MAX bytes.

	ENOENT The current working directory has been unlinked.

	ENOMEM Out of memory.

	ERANGE The  size  argument  is less than the length of the absolute pathname of
			the working directory, including the terminating null byte.  You need to
			allocate a bigger array and try again. 
RETURNS: 0 on error, the number of char printed on successful execution. */
int	ft_pwd(char *cmd)
{
	int		count;
	char	*dir;

	(void)cmd;
	dir = getcwd(NULL, 0);
	if (dir == NULL)
	{
		//handle errno
		return (-1);
	}
	count = ft_printf("%s\n", dir);
	free(dir);
	return (count);
}
