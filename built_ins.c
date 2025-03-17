/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapascal <mapascal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 19:42:26 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/17 17:38:28 by mapascal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_echo(t_cmd cmd, const char **shv, const char **env);
int	ft_cd(t_cmd cmd);
int	ft_pwd(t_cmd cmd);
int	ft_export(t_cmd cmd, char ***shv, char ***env);
int	ft_unset(t_cmd cmd, char ***shv, char ***env);
int	ft_env(char **env);

// static char	*get_full_str(t_token *words, char **shv, char **env)
// {
// 	char	*exp_str;
// 	char	*full_str;

// 	full_str = NULL;
// 	while (words && words->type == TOKEN_WORD)
// 	{
// 		exp_str = expand_string(words->value, (const char **)shv, (const char **)env);
// 		full_str = ft_strjoin_free_space(full_str, exp_str);
// 		if (exp_str == NULL || full_str == NULL)
// 			ft_printfd(2, "echo: malloc failure\n");
// 		free(exp_str);
// 		words = words->next;
// 	}
// 	return (full_str);
// }
/* takes */

static void put_echo_words(int fd, char **words, const char **shv, const char **env)
{
	int i;
	char *exp_str;

	i = 0;

	while (words[i] && words[i][0] != '\0')
	{
		exp_str = expand_string(words[i], shv, env);
		ft_printfd(fd, "%s", exp_str);
		if (words[i+1] && words[i+1][0] != '\0')
			ft_printfd(fd, " ");
		free(exp_str);
		i++;
	}
}
//echo     -n     ahahaha
/* TODO: handle PIPE and tell march that append is the O_FLAG fo read
(either O_WRITE | O_TRUNCATE or O_WRITE | O_APPEND ) */
int	ft_echo(t_cmd cmd, const char **shv, const char **env)
{
	int	fd;

	if (cmd.words[1][0] == '\0')
		return (-1);
	
	fd = 1;
	if (!ft_strncmp("-n", cmd.words[1], 2))
	{
		put_echo_words(fd, &cmd.words[2], shv, env);
		return (1);
	}
	put_echo_words(fd, &cmd.words[1], shv, env);
	ft_printfd(fd, "\n");
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
int	ft_cd(t_cmd cmd)
{
	int		ret;

	if (cmd.words[2] && cmd.words[2][0] != '\0' )
	{
		ft_printfd(2, "minishell: cd: too many argument\n");
		return (-1);
		//better error handling
	}
	ft_printf("new-dir=%s\n", cmd.words[1]);
	ret = chdir(cmd.words[1]);
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
int	ft_pwd(t_cmd cmd)
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
int	ft_export(t_cmd cmd, char ***shv, char ***env)
{
	int	eq;
	int	index;

	if (*env == NULL)
		return (-1);
	//index = is_there(env, cmd);
	eq = ft_strichr(cmd.words[1], '=');
	if (eq != 0)
	{
		*env = var_append(*env, cmd.words[1]); //protect the malloc fail
		return (1);
	}
	if (*shv== NULL)
		return (-1);
	index = is_there((const char **)*shv, cmd.words[1]);
	if (index < 0)
		return (-1);
	*env = var_append(*env, *shv[index]); //remember to drop_index on shv //protect the malloc fail
	return (1);
}

int	ft_unset(t_cmd cmd, char ***shv, char ***env)
{
	int		index;
	char	**mtx;

	if (*env == NULL)
		return (-1);
	index = is_there((const char **)*env, cmd.words[1]);
	if (index >= 0)
	{
		ft_printf("found var in env\n");
		mtx = (char **)drop_index((void **)*env, index);
		if (mtx == NULL)
			return (1);
		*env = mtx;
		return (1);
	}
	index = is_there((const char **)*shv, cmd.words[1]);
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
