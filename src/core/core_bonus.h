/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core_bonus.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 15:06:02 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/15 23:16:20 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CORE_BONUS_H
# define CORE_BONUS_H

# include "core.h"

typedef enum e_logig
{
	LOGIC_AND_OP,
	LOGIC_OR_OP
}			t_logic;

typedef struct s_bush
{
	char			*sub_line;
	t_logic			logic;
	struct s_bush	*true;
	struct s_bush	*false;
	struct s_bush	*next;
}				t_bush;

t_bush	*line_to_bush(char *line);
void	free_bush(t_bush *bush);

int		unbox_the_line_bonus(char *line, char ***vars);

t_bush	*add_new(t_bush **head, char *sub_line, t_logic logic);
t_bush	*fill_bush(char *line);
t_bush	*first_without_brakets(t_bush *curr_bush, int brakets);
t_logic	brakets_out(t_bush *curr_bush, int brakets);
void	print_bush(t_bush *bush);

#endif