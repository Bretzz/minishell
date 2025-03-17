/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 20:30:38 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/17 20:31:00 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_export(t_cmd cmd, char ***shv, char ***env);

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
