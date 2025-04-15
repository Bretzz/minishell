/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_bonus.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 21:10:16 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/15 21:22:12 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_BONUS_H
# define MINISHELL_BONUS_H

#include "minishell.h"

void	handle_line_bonus(char **line, char ***vars);
int		syntax_line_bonus(char **line, char ***vars);
t_cmd	*parse_tokens_bonus(char *line, const char ***vars);
void	bongou_stray_docs_bonus(char *line, const char ***vars);

#endif