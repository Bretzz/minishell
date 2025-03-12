/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapascal <mapascal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 16:05:55 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/12 14:51:47 by mapascal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <term.h>

# include <readline/readline.h>
# include <readline/history.h>

// typedef struct s_list
// {
// 	char		 	*input;
// 	char 			*output;
// 	char 			*command;
// 	struct s_list 	*next;
// }				t_list;

typedef struct s_cmd
{
    char            **argv;         // Nome comando + argomenti
    char            *infile;        // eventuale file di input
    char            *outfile;       // eventuale file di output
    int             append;         // 1 se è ">>", 0 se è ">"
    int             here_doc;       // 1 se è "<<", 0 se è "<"
    struct s_cmd    *next;          // prossimo comando nella pipeline
}   t_cmd;

typedef enum e_token_type
{
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_RED_INPUT,    // "<"
    TOKEN_RED_OUTPUT,   // ">"
    TOKEN_HERE_DOC,          // "<<"
    TOKEN_APPEND,         // ">>"
    TOKEN_SEMICOL,        // ";"
	TOKEN_S_QUOTE,		// '
	TOKEN_D_QUOTE,		// ""
    // ...eventuali altri tipi
}   t_token_type;

typedef struct s_token
{
    t_token_type    type;    // uno dei valori dell'enum
    char            *value;  // la stringa associata (es. "ls", "grep", "file.txt", ecc.)
    struct s_token  *next;
}   t_token;
              

typedef struct s_var
{
	char	*name;
	char	*value;
}				t_var;

//ft_readline.c

char	*ft_readline (const char *prompt);

//pipex_stolen.c

char	*find_env_path(char *cmd, char **env);

//handle stuff

int		handle_vars(char *cmd, char ***shv, char ***env);
char	**var_append(char **mtx, char *var);
int		is_there(const char **mtx, const char *target);

char	*get_value(const char *target, const char **shv, const char **env);
char 	*expand_string(char *str, const char **shv, const char **env);

//ft_execve.c

int		ft_execve(int *fd, const char *cmd, char **env);

//built_ins.c

int		ft_echo(char *cmd, char **shv, char **env);
int		ft_cd(char *cmd);
int		ft_pwd(char *cmd);
int		ft_export(char *cmd, char ***shv, char ***env);
int		ft_unset(char *cmd, char ***shv, char ***env);
int		ft_env(char **env);

//free_stuff.c

int		ft_freentf(const char *s, ...);

//usefull stuff

void	**drop_index(void **mtx, int index);
void	*ft_realloc(void *ptr, size_t old_size, size_t new_size);
size_t	ft_strlen_space(const char *s);
size_t	ft_mtxlen(const void **mtx);

char	*ft_strjoin_free_space_nl(char *s1, char *s2);
size_t	ft_strlen_nl(const char *s);

void	ft_print_charr(const char **arr);

#endif