/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapascal <mapascal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 16:05:55 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/07 23:37:36 by mapascal         ###   ########.fr       */
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
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>

# ifndef DEBUG
#  define DEBUG 0
# endif

# define CLEAN_EXIT -1 /* Return value to close the program */

# define MAX_ARGS 100	/* Max number of arguments to a command */
# define MAX_PATH 4097	/* Max number of cahrs that can compose a path. */

/* I/O REDIRECTION FLAGS */
# define STDL 0		/* Standard Location: STDIN STDOUT execution */
# define FILE 1		/* redirecting either input, output or both into a file */
# define PIPE 2		/* reding/writing from/into a pipe */
# define HERE_DOC 3 /* recieving input from an HERE_DOC */

extern int g_last_sig;

typedef struct s_cmd
{
    char	*words[MAX_ARGS];	// Array di argomenti (comando + parametri)
    int		fd[2];
	char	infile[MAX_PATH];	// ! ! ! REMOVE ! ! ! File di input, se presente
    char	outfile[MAX_PATH];	// ! ! ! REMOVE ! ! ! File di output, se presente
    int		append;				// ! ! ! REMOVE ! ! ! Flag: O_WRONLY|O_CREAT|O_APPEND per ">>" oppure O_WRONLY|O_CREAT|O_TRUNC per ">"
	int		redir[2];			// (redir[0]: input, redir[1]: output) Flag: PIPE_OUT: pipe dopo il comando, PIPE_IN: pipe prima del comando, HERE_DOC: heredoc prima del comando, FILE: input or output file, STDL: nessuna redirection.
	int		close_me;
}	t_cmd;

/* PARSING */

t_cmd	*parse_tokens(char *line, const char ***vars);
char	*syntax_line(char *line);
char	*drop_comment(char *line);

int		ft_cmdlen(t_cmd *cmd_array);
void	free_cmd(t_cmd *cmd_arr);

//ft_readline.c

char	*ft_readline (const char *prompt);

/* VARIABLES */

int		handle_vars(t_cmd cmd, char ***vars);

char 	*expand_string(char *str, const char ***vars);
char 	*just_expand_string(char *str, const char ***vars);

size_t	ft_varlen(const char *s);
size_t	ft_mtxlen(const void **mtx);

//here_doc.c

int		here_doc(char *limiter, const char ***vars);
void	close_docs(void);

/* FINAL EXECUTION */

int		execute_command(char *line, t_cmd *cmd, char ***vars);
int		execute_pipeline(char *line, t_cmd *cmd_arr, char ***vars);
int		ft_wifexited(pid_t pid);

t_cmd	pop_arg(t_cmd cmd, int index);

/* BUILTINS */

int		ft_echo(int *fd, t_cmd cmd);
int		ft_cd(int *fd, t_cmd cmd, char ***vars);
int		ft_pwd(int *fd, t_cmd cmd);
int		ft_export(int *fd, t_cmd cmd, char ***vars);
int		ft_unset(int *fd, t_cmd cmd, char ***vars);
int		ft_env(int *fd, const char ***vars);

/* SIGNALS */

void	idle_initializer(void);
void    input_initializer(void);
void    doc_initializer(void);

/* UTILS */

void	*ft_realloc(void *ptr, size_t old_size, size_t new_size);

size_t	ft_strlen_space(const char *s); //replace with ft_varlen when measuring vars
size_t	ft_strlen_nl(const char *s);

char	*ft_strjoin_free_space(char *s1, char *s2);
char	*ft_strjoin_free_nl(char *s1, char *s2);
char	*ft_strjoin_free_nl_space(char *s1, char *s2);

void	ft_print_charr(const char **arr);

/* CLEANUP */

void	safeclose(int fd);
void	multicose(int *fd);
void	free_mtx(void **mtx);
void	free_mtx_mtx(void ***mtx);

#endif