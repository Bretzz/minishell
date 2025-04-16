/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core_bonus.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 15:06:02 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/16 22:26:22 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CORE_BONUS_H
# define CORE_BONUS_H

# include "core.h"

void	clean_exit_bonus(t_cmd *cmd_arr, t_bush *bush, char ***vars, int code);

t_bush	*line_to_bush(char *line);
void	free_bush(t_bush *bush);

int		unbox_the_line_bonus(char *line, char ***vars);

t_bush	*add_new(t_bush **head, char *sub_line, t_logic logic);
t_bush	*fill_bush(char *line);
t_bush	*first_without_brakets(t_bush *curr_bush, int brakets);
t_logic	brakets_out(t_bush *curr_bush, int brakets);
void	print_bush(t_bush *bush);

#endif