/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 14:57:25 by totommi           #+#    #+#             */
/*   Updated: 2025/04/30 17:28:22 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,			// |
	TOKEN_SEMICOL,		// ;
	TOKEN_AND_OP,		// &&
	TOKEN_OR_OP,		// ||
	TOKEN_RED_INPUT,	// <
	TOKEN_RED_OUTPUT,	// >
	TOKEN_HERE_DOC,		// <<
	TOKEN_APPEND,		// >>
	TOKEN_S_QUOTE,		// '
	TOKEN_D_QUOTE,		// "
	TOKEN_OPEN_BR,
	TOKEN_CLOSE_BR
	// ...eventuali altri tipi
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;	// uno dei valori dell'enum
	char			*value;	// stringa associata (es: "ls", ">>", "file", ...)
	char			*not_expanded;
	struct s_token	*next;
}	t_token;

/* FUNCTIONS FOR INTERNAL PURPOUSE */

//tokens

t_token			*tokenizer(char *line);
void			free_tokens(t_token *tokens);

//syntax

int				syntax_tokens(t_token *tokens);

char			colon_pipe_check(t_token *colonipe, t_token *prev);
char			redir_check(t_token *redir);
char			word_check(t_token *word, t_token *prev);
char			is_error(char *err_code);

//token expansion

int				expand_tokens(t_token *tokens, const char ***vars);

//closer of pipes and quotes

char			*append_line(char *line, t_token_type last_type, char ***vars);

//syntax_utils

int				is_closed(char *line);
char			get_last_quote(char *line);

//token_utils

t_token_type	get_last_type(t_token *tokens);
t_token_type	get_next_operator(const char *line, int *i);
int				is_operator(const char *line, int i);
void			skip_spaces(const char *line, int *i);

void			print_tokens(t_token *tokens);
void			process_redirection(t_token **tokens,
					t_cmd *current_cmd, const char ***vars);
void			create_redir_out(t_token *tokens);

void			free_cmd(t_cmd *cmd_arr);
int				cmds_count(const t_token *tokens);
void			raccattagarbage(t_cmd *cmd_arr);
int				ft_cmdlen(t_cmd *cmd_array);

#endif