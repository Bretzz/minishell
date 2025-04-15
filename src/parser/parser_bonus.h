/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_bonus.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 17:11:03 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/15 23:24:57 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_BONUS_H
# define PARSER_BONUS_H

# include "parser.h"

/* TOKENIZER_BONUS */

t_token			*tokenizer_bonus(char *line);
t_token_type	get_next_operator_bonus(const char *line, int *i);
int				is_operator_bonus(const char *line, int i);

/* SYNTAX_TOKENS_BONUS */

int				syntax_tokens_bonus(t_token *tokens);
char			is_error_bonus(char *err_code);
char			braket_check(t_token *braket, t_token *prev, int is_open);

#endif