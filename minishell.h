/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 16:05:55 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/16 22:04:27 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include "mtx_safe.h"
# include <dirent.h>
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
# define MAX_PATH 4096	/* Max number of cahrs that can compose a path. */

/* I/O REDIRECTION FLAGS */
# define STDL 0		/* Standard Location: STDIN STDOUT execution */
# define FILE 1		/* redirecting either input, output or both into a file */
# define PIPE 2		/* reding/writing from/into a pipe */
# define HERE_DOC 3	/* recieving input from an HERE_DOC */

extern int	g_last_sig;

typedef struct s_cmd
{
	char	*words[MAX_ARGS];		// Array di argomenti (comando + parametri)
	int		fd[2];
	int		close_me;
	char	infile[MAX_PATH + 1];	// File di input, se presente
	char	outfile[MAX_PATH + 1];	// File di output, se presente
	int		append;					// Flag: o_flag composete per ">>" o ">"
	int		redir[2];				// Flag: dove il comando redirecta
	int		parse_code;
}	t_cmd;

/* INIT */

char	**env_init(char **his_env);
void	stdin_init(int argc, char **argv, char ***vars);
void	sigflag_init(char ***vars);
void	vars_init(char **__environ, char ***vars);

/* HANDLE LINE */

void	handle_line(char **line, char ***vars);

/* PARSING */

t_cmd	*parse_tokens(char *line, const char ***vars);
int		syntax_line(char **line, char ***vars);
char	*drop_comment(char *line);

void	free_cmd(t_cmd *cmd_arr);
int		ft_cmdlen(t_cmd *cmd_array);

/* VARIABLES */

int		handle_vars(t_cmd cmd, char ***vars);

char	*expand_string(char *str, const char ***vars);
char	*just_expand_string(char *str, const char ***vars);

char	*wide_search(char *str, const char ***vars);

//here_doc.c

int		here_doc(char *limiter, const char ***vars);
void	close_docs(void);
void	bongou_stray_docs(char *line, const char ***vars);

/* FINAL EXECUTION */

int		execute_command(char *line, t_cmd *cmd, char ***vars);
int		execute_pipeline(char *line, t_cmd *cmd_arr, char ***vars);
int		ft_wifexited(pid_t pid);
void	ft_perror(char *cmd, char *strerr, int *exit_code, int code);

t_cmd	pop_arg(t_cmd cmd, int index);

/* BUILTINS */

int		ft_echo(int *fd, t_cmd cmd);
int		ft_cd(int *fd, t_cmd cmd, char ***vars);
int		ft_pwd(int *fd, t_cmd cmd);
int		ft_export(int *fd, t_cmd cmd, char ***vars);
int		ft_unset(int *fd, t_cmd cmd, char ***vars);
int		ft_env(int *fd, const char ***vars);
int		ft_exit(int *fd, t_cmd cmd, char ***vars);

/* SIGNALS */

void	idle_initializer(void);
void	runtime_initializer(void);
void	input_initializer(void);

/* UTILS */

//cool stuff
void	*ft_realloc(void *ptr, size_t old_size, size_t new_size);
char	*drop_string(char *str, int start, size_t len);
int		is_white(char *str);
int		is_number(char *str);
char	*trim_back_nl(char *str);
int		chrnum(char *str, int c);

char	*get_safe_line(int fd);

//more cool stuff
int		ft_strbcmp(const char *s1, const char *s2, size_t n);
int		ft_strcmp_nocase(const char *s1, const char *s2);
void	swap_ptr(void **ptr1, void **ptr2);

//strange strlen
size_t	ft_varlen(const char *s);
size_t	ft_mtxlen(const void **mtx);
size_t	ft_strlen_nl(const char *s);
size_t	ft_strlen_space(const char *s);

//strjoin
char	*ft_strjoin_free_space(char *s1, char *s2);
char	*ft_strjoin_free_nl(char *s1, char *s2);
char	*ft_strjoin_free_nl_space(char *s1, char *s2);

/* CLEANUP */

void	safeclose(int fd);
void	multicose(int *fd);
void	free_mtx(void **mtx);
void	free_mtx_mtx(void ***mtx);

#endif