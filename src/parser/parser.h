/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 14:57:25 by totommi           #+#    #+#             */
/*   Updated: 2025/04/06 20:07:16 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,			// |
	TOKEN_RED_INPUT,    // <
	TOKEN_RED_OUTPUT,   // >
	TOKEN_HERE_DOC,     // <<
	TOKEN_APPEND,       // >>
	TOKEN_SEMICOL,      // ;
	TOKEN_S_QUOTE,		// '
	TOKEN_D_QUOTE,		// "
	// ...eventuali altri tipi
}   t_token_type;

typedef struct s_token
{
	t_token_type    type;    // uno dei valori dell'enum
	char            *value;  // la stringa associata (es. "ls", "grep", "file.txt", ecc.)
	struct s_token  *next;
}   t_token;			  

/* FUNCTIONS FOR INTERNAL PURPOUSE */

//tokens
t_token			*tokenizer(char *line, const char ***vars);
void			free_tokens(t_token *tokens);

void			skip_spaces(const char *line, int *i);
int				is_operator(const char *line, int i);
t_token_type	pipe_or_die(const char *line, int *i);
t_token_type	get_next_operator(const char *line, int *i);
//char			*get_next_word(const char *line, int *i);
int				add_token(t_token **tokens, t_token_type type, char *value);
char 			*get_rekd(t_token_type type);

void			print_tokens(t_token *tokens);

int				syntax_check(char *line);

#endif