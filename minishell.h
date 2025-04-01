/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 16:05:55 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/01 13:42:51 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include "mtx_safe.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <term.h>

# include <readline/readline.h>
# include <readline/history.h>

# define MAX_ARGS 100		/* Max number of arguments to a command */
# define VARNAME 1024	/* Max length of a shell variable's name. */
/* NOTE: currently we can take a var value of any length. */

/* PROGRAM LOCATION FLAGS */
# define STDL 0		/* Standard Location: STDIN STDOUT execution */
# define FILE 1		/* redirecting either input, output or both into a file */
# define PIPE 2		/* reding/writing from/into a pipe */
# define HERE_DOC 3 /* recieving input from an HERE_DOC */

extern int g_last_sig;

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


typedef struct s_cmd
{
    char	*words[MAX_ARGS];   // Array di argomenti (comando + parametri)
    char	infile[1024];       // File di input, se presente
    char	outfile[1024];      // File di output, se presente
    int		append;             // Flag: O_WRONLY|O_CREAT|O_APPEND per ">>" oppure O_WRONLY|O_CREAT|O_TRUNC per ">"
	int		redir[2];			// (redir[0]: input, redir[1]: output) Flag: PIPE_OUT: pipe dopo il comando, PIPE_IN: pipe prima del comando, HERE_DOC: heredoc prima del comando, FILE: input or output file, STDL: nessuna redirection.
}	t_cmd;


typedef struct s_var
{
	char	*name;
	char	*value;
}				t_var;

// parsing
t_cmd	*parse_tokens(char *line);
int		ft_cmdlen(t_cmd *cmd_array);
void	free_cmd(t_cmd *cmd_arr);

//tokens
t_token	*tokenizer(char *line);
void	free_tokens(t_token *tokens);

void	skip_spaces(const char *line, int *i);
int	is_operator(const char *line, int i);
t_token_type	pipe_or_die(const char *line, int *i);
t_token_type	get_next_operator(const char *line, int *i);
char	*remove_quotes(char *str);
char	*get_next_word(const char *line, int *i);
void	add_token(t_token **tokens, t_token_type type, char *value);
t_token	*tokenizer(char *line);
void	print_tokens(t_token *tokens);
char *get_rekd(t_token_type type);


//ft_readline.c

char	*ft_readline (const char *prompt);

//pipex_stolen.c

char	*find_env_path(char *cmd, char **env);

//handle stuff

int		handle_vars(t_cmd cmd, char ***vars);
char	**var_append(char **mtx, char *var);
char	**setnum(char **mtx, const char *target, int value);

char 	*expand_string(char *str, const char ***vars);

char	*get_value(const char *target, const char **mtx);
int		is_there(const char **mtx, const char *target);
int		var_is_valid(const char *var);
int		var_check(const char *var);

//ft_execve.c

int		ft_execve(int *fd, t_cmd cmd, char **env);
pid_t	wrapper(int *fd, int *oldfd, t_cmd cmd);
int		miniwrapper(int *fd, int *oldfd, t_cmd cmd);

//built_ins.c

int		ft_echo(int *fd, t_cmd cmd, const char ***vars);
int		ft_cd(int *fd, t_cmd cmd);
int		ft_pwd(int *fd, t_cmd cmd);
int		ft_export(int *fd, t_cmd cmd, char ***vars);
int		ft_unset(int *fd, t_cmd cmd, char ***vars);
int		ft_env(int *fd, const char ***vars);

//free_stuff.c

int		ft_freentf(const char *s, ...);

//usefull stuff

void	**drop_index(void **mtx, int index);
void	*ft_realloc(void *ptr, size_t old_size, size_t new_size);

size_t	ft_strlen_space(const char *s); //replace with ft_varlen when measuring vars
size_t	ft_varlen(const char *s);

size_t	ft_mtxlen(const void **mtx);

char	*ft_strjoin_free_space(char *s1, char *s2);
char	*ft_strjoin_free_nl_space(char *s1, char *s2);

void	ft_print_charr(const char **arr);

void	safeclose(int fd);
void	multicose(int *fd);


void	sig_initializer(void);



#endif