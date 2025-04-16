/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_exit_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 22:23:38 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/16 22:28:01 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"
#include "core_bonus.h"

void	clean_exit_bonus(t_cmd *cmd_arr, t_bush *bush, char ***vars, int code)
{
	free_cmd(cmd_arr);
	mtx_free(vars[0]);
	mtx_free(vars[1]);
	free(bush->line);
	free_bush(bush);
	exit(code);
}
void	clean_exit(t_cmd *cmd_arr, char *line, char ***vars, int code)
{
	free_cmd(cmd_arr);
	mtx_free(vars[0]);
	mtx_free(vars[1]);
	free(line);
	exit(code);
}

