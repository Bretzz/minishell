/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_bonus.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 21:10:16 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/16 16:52:46 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_BONUS_H
# define MINISHELL_BONUS_H

#include "minishell.h"

typedef enum e_logig
{
	LOGIC_AND_OP,
	LOGIC_OR_OP
}			t_logic;

typedef struct s_bush
{
	char			*line;
	char			*sub_line;
	t_logic			logic;
	struct s_bush	*true;
	struct s_bush	*false;
	struct s_bush	*next;
}				t_bush;

void	free_bush(t_bush *bush);

void	handle_line_bonus(char **line, char ***vars);
int		syntax_line_bonus(char **line, char ***vars);
t_cmd	*parse_tokens_bonus(char *line, const char ***vars);
void	bongou_stray_docs_bonus(char *line, const char ***vars);

int		execute_command_bonus(char *line, t_bush *bush, t_cmd *cmd, char ***vars);
int		execute_pipeline_bonus(char *line, t_bush *bush, t_cmd *cmd, char ***vars);

char	*expand_string_bonus(char *str, const char ***vars);
// char	*just_expand_string_bonus(char *str, const char ***vars);
char	*get_wild_value(size_t *i, char *str);

#endif