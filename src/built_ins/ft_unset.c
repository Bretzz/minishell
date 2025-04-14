/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 20:31:16 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/14 15:30:23 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "built_ins.h"

int	ft_unset(int *fd, t_cmd cmd, char ***vars);

static void	mass_drop(char ***vars, char *var)
{
	char	*name;
	int		index;
	int		i;

	name = vstr_getname(var, NULL, 0);
	if (name == NULL)
	{
		write(STDERR_FILENO, "minishell: malloc failure\n", 26);
		return ;
	}
	i = 1;
	while (i >= 0)
	{
		if (vars[i] != NULL)
		{
			index = mtx_getindex(name, vars[i]);
			if (index >= 0)
				mtx_safedel(index, vars[i]);
		}
		i--;
	}
}

int	ft_unset(int *fd, t_cmd cmd, char ***vars)
{
	int		exit_status;
	int		i;

	exit_status = 0;
	i = 1;
	while (cmd.words[i] && cmd.words[i][0] != '\0')
	{
		if (vstr_name_is_valid(cmd.words[i]) <= 0)
		{
			ft_printfd(STDERR_FILENO, "minishell: unset: `%s': \
not a valid identifier\n", cmd.words[i]);
			exit_status = 1;
		}
		else
			mass_drop(vars, cmd.words[i]);
		i++;
	}
	safeclose(fd[1]);
	return (exit_status);
}
