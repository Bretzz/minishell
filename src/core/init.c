/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 20:02:41 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/14 20:05:10 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "core.h"

char	**env_init(char **his_env);
void	stdin_init(int argc, char **argv, char ***vars);
void	sigflag_init(char ***vars);
void	vars_init(char **__environ, char ***vars);

/* if the SHLVL var is present increments it's value by 1. */
static void	level_up(char ***env)
{
	char	value[12];
	int		new_val;

	mtx_findval("SHLVL", value, sizeof(value), *env);
	if (value[0] == '\0')
		new_val = 1;
	else
		new_val = ft_atoi(value) + 1;
	*env = mtx_setnum("SHLVL", new_val, *env);
}

/* copies the enviroment passed as parameters and returns
the newly initialized matrix. */
/* ! ! ! SETUP SHLVL, PWD, TMPDIR ! ! ! */
char	**env_init(char **his_env)
{
	size_t	i;
	char	**my_env;

	if (his_env == NULL)
		return (NULL);
	my_env = mtx_init();
	if (my_env == NULL)
		return (NULL);
	i = 0;
	while (his_env[i] != NULL)
	{
		my_env = mtx_vstr_copy(his_env[i], my_env);
		i++;
	}
	level_up(&my_env);
	return (my_env);
}

/* if a parameter is passed and it's a valid file, then
a dup2(3) call is performed so that STDIN_FILENO is pointing
to an fd open(3)ed on that file. */
void	stdin_init(int argc, char **argv, char ***vars)
{
	int	fd;
	int	exit_code;

	if (argc <= 1)
		return ;
	exit_code = 0;
	if (ft_strlen(argv[1]) > MAX_PATH)
		ft_perror(argv[1], "File name too long", &exit_code, 126);
	if (access(argv[1], F_OK) != 0)
		ft_perror(argv[1], "No such file or directory", &exit_code, 127);
	if (access(argv[1], R_OK) != 0)
		ft_perror(argv[1], "Permission denied", &exit_code, 126);
	if (exit_code != 0)
		clean_exit(NULL, NULL, vars, exit_code);
	fd = open(argv[1], O_RDONLY);
	if (fd < 0 || dup2(fd, STDIN_FILENO) < 0)
	{
		safeclose(fd);
		ft_perror(argv[1], "Unknown error", NULL, 1);
		clean_exit(NULL, NULL, vars, EXIT_FAILURE);
	}
	close(fd);
}

/* sets the sigflag 'private space' of vars[0]
based on the attributes of the STDIN_FILENO. */
void	sigflag_init(char ***vars)
{
	if (isatty(STDIN_FILENO))
	{
		mtx_setdata(1, vars[0], 2);
	}
	else
	{
		mtx_setdata(0, vars[0], 2);
	}
}

/* Initializes both vars[0] and vars[1], then setsup vars[1]
to be the env. if something went wrong an exit(3)s with
exit code EXIT_FAILURE */
void	vars_init(char **__environ, char ***vars)
{
	ft_bzero(vars, 2 * sizeof(char **));
	vars[1] = env_init(__environ);
	vars[0] = mtx_init();
	if (!vars[0] || !vars[1])
	{
		mtx_free(vars[0]);
		mtx_free(vars[1]);
		exit(EXIT_FAILURE);
	}
}
