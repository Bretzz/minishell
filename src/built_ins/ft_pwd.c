/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 20:29:59 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/21 15:10:02 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pwd(int *fd, t_cmd cmd);

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
int	ft_pwd(int *fd, t_cmd cmd)
{
	char	*dir;

	(void)cmd;
	safeclose(fd[0]);
	dir = getcwd(NULL, 0);
	if (dir == NULL)
	{
		//handle errno
		return (1); //return errno
	}
	ft_printfd(fd[1], "%s\n", dir);
	free(dir);
	safeclose(fd[1]);
	return (0);
}
