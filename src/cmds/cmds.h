/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 12:15:55 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/16 11:31:17 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMDS_H
# define CMDS_H

# define CREATE 0	/* open_doc() creation flag. */
# define RESET 1	/* reset the doc_num static variable. */
# define GETNUM 2	/* open_doc() information flag. */

typedef struct s_garb
{
	pid_t	pid;
	int		fd[2];
	int		pipefd[2];
	t_cmd	*cmd;
	char	*line;
	char	***vars;
}			t_garb;

/* BUILTIN BRIDGE (BB) */

int		is_builtin(char *cmd);
int		exec_builtin(int *fd, t_cmd cmd, char ***vars);
void	builtin_and_die(int *fd, int index, t_cmd *cmd_arr, char ***vars);

/* HERE_DOC UTILS */

int		open_doc(char flag);
void	close_docs(void);
int		read_doc(int doc_num);
char	*get_doc_path(int doc_num, char *buff, size_t size);
char	*prompt_safe_line(const char *prompt, char sig_flag);
int		doc_exp_write(int fd, char exp_flag, char *line, const char ***vars);

//pipex_stolen.c

int		ft_execve(int *fd, t_cmd cmd, char **env);
char	*find_env_path(char *cmd, char **env);

/* EXE_CUTE, (exec helpers) */

int		dummy_in_pipe(void);
t_cmd	pop_arg(t_cmd cmd, int index);
int		ft_wifexited(pid_t pid);
int		is_directory(const char *path);

/* EXE_REALLY_CUTE (pipelint helpers) */

int		mass_wait(t_garb *garb, size_t len);
void	father_closes(t_cmd *cmd, t_garb *garb, int index, size_t len);
void	child_closes(t_cmd *cmd, t_garb *garb, int index, size_t len);
void	single_execute(int *execfd, int index, t_cmd *cmd, char ***vars);

/* INPUT REDIRECTIONS */

int		pipeline_redir_input(t_cmd *cmd, t_garb *garb, int index, size_t len);
int		pipeline_redir_output(t_cmd *cmd, t_garb *garb, int index, size_t len);
int		command_redir_input(t_cmd *cmd);
int		command_redir_output(t_cmd *cmd);

#endif