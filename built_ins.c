/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 19:42:26 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/13 15:45:15 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_echo(t_cmd cmd, char **shv, char **env);
int	ft_cd(char *cmd);
int	ft_pwd(char *cmd);
int	ft_export(char *cmd, char ***shv, char ***env);
int	ft_unset(char *cmd, char ***shv, char ***env);
int	ft_env(char **env);

static char	*get_full_str(t_token *words, char **shv, char **env)
{
	char	*exp_str;
	char	*full_str;

	full_str = NULL;
	while (words && words->type == TOKEN_WORD)
	{
		exp_str = expand_string(words->value, (const char **)shv, (const char **)env);
		full_str = ft_strjoin_free_space(full_str, exp_str);
		if (exp_str == NULL || full_str == NULL)
			ft_printfd(2, "echo: malloc failure\n");
		free(exp_str);
		words = words->next;
	}
	return (full_str);
}

static int	put_words_fd(int fd, t_token *words, char **shv, char **env)
{
	char **full_str;
	
	full_str = get_full_str(words, shv, env);
	if (full_str == NULL)
		return (-1);
	ft_printfd(fd, "%s", full_str);
	return (free(full_str), 1);
}

//echo     -n     ahahaha
/* TODO: handle PIPE and tell march that append is the O_FLAG fo read
(either O_WRITE | O_TRUNCATE or O_WRITE | O_APPEND ) */
int	ft_echo(t_cmd cmd, char **shv, char **env)
{
	int	fd;

	if (cmd.words->value == '\0')
		return (-1);
	
	fd = 1;
	//get output fd
	if (ft_strncmp("STDOUT", cmd.outfile, 5) != 0
		&& ft_strncmp("PIPE", cmd.outfile, 4) != 0)
		if (access(cmd.outfile, cmd.append) == 0)
			fd = open(cmd.outfile, cmd.append);

	if (ft_strncmp("-n", cmd.words->value, 2) != 0)
	{
		put_words_fd(fd, cmd.words, shv, env);
		ft_printfd(fd, "\n");
		return (1);
	}
	put_words_fd(fd, cmd.words->next, shv, env);
	if (fd > 2)
		close(fd);
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


//env no, shv no
//env no, shv si
//env si, shv no
//env si, shv si
int	ft_export(char *cmd, char ***shv, char ***env)
{
	int	eq;
	int	index;

	cmd += 7;
	if (*env == NULL)
		return (-1);
	//index = is_there(env, cmd);
	eq = ft_strichr(cmd, '=');
	if (eq != 0)
	{
		*env = var_append(*env, cmd); //protect the malloc fail
		return (1);
	}
	if (*shv== NULL)
		return (-1);
	index = is_there((const char **)*shv, cmd);
	if (index < 0)
		return (-1);
	*env = var_append(*env, *shv[index]); //remember to drop_index on shv //protect the malloc fail
	return (1);
}

int	ft_unset(char *cmd, char ***shv, char ***env)
{
	int		index;
	char	**mtx;

	cmd += 6;
	if (*env == NULL)
		return (-1);
	index = is_there((const char **)*env, cmd);
	if (index >= 0)
	{
		ft_printf("found var in env\n");
		mtx = (char **)drop_index((void **)*env, index);
		if (mtx == NULL)
			return (1);
		*env = mtx;
		return (1);
	}
	index = is_there((const char **)*shv, cmd);
	if (index >= 0)
	{
		ft_printf("found var in shv\n");
		mtx = (char **)drop_index((void **)*shv, index);
		if (mtx == NULL)
			return (1);
		*shv = mtx;
		return (1);
	}
	ft_printf("no var found\n");
	//ft_printf("I should do something, but I don't :D\n");
	return (1);
}

int	ft_env(char **env)
{
	int	i;

	if (env == NULL)
	{
		ft_printf("%p\n", env);
		return (-1);
	}
	i = 0;
	while (env[i] != NULL)
	{
		ft_printf("%s\n", env[i]);
		i++;
	}
	return (1);
}
