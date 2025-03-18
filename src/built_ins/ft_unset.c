/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 20:31:16 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/18 19:42:40 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_unset(int *fd, t_cmd cmd, char ***shv, char ***env);

int	ft_unset(int *fd, t_cmd cmd, char ***shv, char ***env)
{
	int		index;
	char	**mtx;

	multicose(fd);
	if (*env == NULL) //should never happen
		return (-1);
	index = is_there((const char **)*env, cmd.words[1]);
	if (index >= 0)
	{
		ft_printf("found var in env\n");
		mtx = (char **)drop_index((void **)*env, index);
		if (mtx == NULL)
			return (1);
		*env = mtx;
		return (0);
	}
	index = is_there((const char **)*shv, cmd.words[1]);
	if (index >= 0)
	{
		ft_printf("found var in shv\n");
		mtx = (char **)drop_index((void **)*shv, index);
		if (mtx == NULL)
			return (1);
		*shv = mtx;
		return (0);
	}
	ft_printf("no var found\n");
	//ft_printf("I should do something, but I don't :D\n");
	return (1);
}
