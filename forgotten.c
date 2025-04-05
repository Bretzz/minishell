/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forgotten.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totommi <totommi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 19:39:59 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/05 15:15:50 by totommi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		ft_putchar(int c);

char	*get_value(const char *target, const char **mtx);
int		var_is_valid(const char *var);
int		var_check(const char *var);

char	**setnum(char **mtx, const char *target, int value);
static char	**replace_var(int index, char **mtx, char *var);
char	**var_append(char **mtx, char *var);

int		is_there(const char **mtx, const char *target);
void	**drop_index(void **mtx, int index);

pid_t	wrapper(int *fd, int *oldfd, t_cmd cmd);
int		miniwrapper(int *fd, int *oldfd, t_cmd cmd);

/* BEGINNING OF WRAPPER */

/* takes argv and a pipedes as parameters.
writes "pipe heredoc> " to the stdout,
writes the stdin to the write end of the pipe,
breaks when the stdin is exactly argv[2] */
// static void	here_doc(int where, char *limiter, int *pipefd)
// {
// 	char	*line;
// 	char	prompt[15];

// 	ft_bzero(prompt, 15);
// 	if (where == PIPE)
// 		ft_strlcpy(prompt, "pipe heredoc> ", 15);
// 	else
// 		ft_strlcpy(prompt, "heredoc> ", 10);
// 	ft_printf("%s", prompt);
// 	line = get_next_line(STDIN_FILENO);
// 	while (/* line != NULL &&  */!(line && !ft_strncmp(limiter, line, ft_strlen(limiter))
// 			&& line[ft_strlen(limiter)] == '\n'))
// 	{
// 		if (line != NULL)
// 		{
// 			write(pipefd[1], line, ft_strlen(line));
// 			free(line);
// 		}
// 		else
// 			ft_printf("\nminishell: is weak...\n");
// 		ft_printf("%s", prompt);
// 		line = get_next_line(STDIN_FILENO);
// 	}
// 	//ft_printf("\n");
// 	free(line);
// 	close(pipefd[1]);
// }

/* defaults returns STDIN_FILENO,
if an input file is found, returns it's fd after a successful open(3) call. */
static int	redir_input(int *oldfd, t_cmd cmd)
{
	//int	pipefd[2];
	
	//ft_printf("infile:%s, redir=%d\n", cmd.infile, cmd.redir[0]);
	if (cmd.redir[0] == PIPE)
	{
		return (oldfd[0]);
	}
	// if (cmd.redir[0] == HERE_DOC)
	// {
	// 	if (pipe(pipefd) < 0)
	// 	{
	// 		ft_printfd(2, "minishell: pippe error to be handled\n");
	// 		return (-1);
	// 	}
	// 	if (cmd.redir[1] == PIPE)
	// 		here_doc(PIPE, cmd.infile, pipefd);
	// 	else
	// 		here_doc(STDL, cmd.infile, pipefd);
	// 	return (pipefd[0]);
	// }
	if (cmd.redir[0] == FILE || cmd.redir[0] == HERE_DOC)
	{
		safeclose(oldfd[0]);
		return (cmd.fd[0]);
		// if (access(cmd.infile, R_OK) == 0)
		// 	return(open(cmd.infile, O_RDONLY));
		// return (-1);
	}
	return (STDIN_FILENO);
}

/* defaults returns STDIN_FILENO,
if an output file is found, returns it's fd after a successful open(3) call. */
static int	redir_output(int *pipefd, t_cmd cmd)
{
	//int	pipefd[2];
	
	//ft_printf("outfile:'%s', redir=%d\n", cmd.outfile, cmd.redir[1]);
	if (cmd.redir[1] == PIPE)
	{
		if (pipe(pipefd) < 0)
		{
			ft_printfd(2, "minishell: pippe error to be handled\n");
			return (-1);
		}
		/* oldfd[0] = pipefd[0];
		oldfd[1] = pipefd[1]; */
		return (pipefd[1]);
	}
	if (cmd.redir[1] == FILE)
	{
		return (cmd.fd[1]);
		// //if (access(cmd.outfile, W_OK) == 0)
		
		// if (access(cmd.outfile, F_OK) == -1 || access(cmd.outfile, W_OK) == 0)
		// 	return(open(cmd.outfile, cmd.append, 0644));
		// return (-1);
	}
	return (STDOUT_FILENO);
}

/* takes a fd pointer to a stack allocated mem of size 2 (int fd[2]),
and a cmd struct as parameters.
Assign the redirected input and output to fd[0] and fd[1] respectively,
and copies pipefd into oldfd (if we piped).
It forks, then 
FATHER:
	the FATHER close no-needed fds,
	then returns the pid of the child,
CHILD:
	the CHILD closes no-needed fds then 
	calls dup2(3) on the redirections (or throws errors).
RETURNS: the pid of the child (father) or 0 (child). */
pid_t	wrapper(int *fd, int *oldfd, t_cmd cmd)
{
	pid_t		pid;
	//static int	oldfd[2]; // a copy of the previous pipe
	int			pipefd[2]; // fds of the current pipe

	pipefd[0] = 0;
	pipefd[1] = 1;
	fd[0] = redir_input(oldfd, cmd);
	fd[1] = redir_output(pipefd, cmd);
	// ft_printf("----oldfd=[%d,%d]\n", oldfd[0], oldfd[1]);
	// ft_printf("---pipefd=[%d,%d]\n", pipefd[0], pipefd[1]);
	pid = fork();
	if (pid < 0)
		return (-1); //send error
	else if (pid != 0)
	{
		if (cmd.redir[0] == PIPE) // the father doesn't need the READ end of the pipe since just the child's gonna read on it
		{
			// ft_printf("----father closed %d\n", oldfd[0]);
			close(oldfd[0]);
		}
		if (cmd.redir[1] == PIPE) // the father doesn't need the WRITE end of the pipe since the child's gonna write on it
		{
			// ft_printf("----father closed %d\n", pipefd[1]);
			close(pipefd[1]);
		}
		if (cmd.redir[1] == FILE && fd[1] > 2)
			close(fd[1]);
		if ((cmd.redir[0] == FILE || cmd.redir[0] == HERE_DOC) && fd[0] > 2)
			close(fd[0]);
		ft_memcpy(oldfd, pipefd, 2 * sizeof(int)); // backup of old pipe
		return (pid);
	}
	// if (cmd.redir[0] == PIPE) // the father doesn't need the READ end of the pipe since jsut the child's gonna read on it
	// 	close(oldfd[1]);
	if (cmd.redir[1] == PIPE) // the child doesn't need the READ end of the pipe since he's just gonna write on it
	{
		// ft_printf("----child closed %d\n", pipefd[0]);
		close(pipefd[0]);
	}
	/* dup2(fd[0], STDIN_FILENO);
	dup2(fd[1], STDOUT_FILENO); */
	// no need for backups, he's gonna die anyway
	return (pid);
}

int	miniwrapper(int *fd, int *oldfd, t_cmd cmd)
{
	int	pipefd[2]; // fds of the current pipe

	pipefd[0] = 0;
	pipefd[1] = 1;
	fd[0] = redir_input(oldfd, cmd);
	fd[1] = redir_output(pipefd, cmd);
	// ft_printf("----oldfd=[%d,%d]\n", oldfd[0], oldfd[1]);
	// ft_printf("---pipefd=[%d,%d]\n", pipefd[0], pipefd[1]);
	// ft_printf("--finalfd=[%d,%d]\n", fd[0], fd[1]);

	ft_memcpy(oldfd, pipefd, 2 * sizeof(int));
	return (0);
}

/* END OF WRAPPER */

/* checks weather the target var is present or not.
RETURNS: the index of the var, -1 if not present or mtx/target is NULL.
EXPECTED: string 'varname' or 'varname='*/
int	is_there(const char **mtx, const char *target)
{
	char	*my_tar;
	size_t	tar_len;
	int		cmp;
	int		i;

	if (!mtx || !target)
		return (-1);
	i = 0;
	while (target[i] && target[i] != '=')
		i++;
	// if (*target == '$')	//both $var and var=value behaviour should be impelented
		//code here
	my_tar = ft_substr(target, 0, i);
	tar_len = ft_strlen(my_tar);
	i = 0;
	while (mtx[i] != NULL)
	{
		cmp = ft_strncmp(mtx[i], my_tar, tar_len + 1);
		if (cmp == 0 || cmp == '=')
			return (free(my_tar), i);
		i++;
	}
	free(my_tar);
	return(-1);
}

/* takes a matrix and an index as parameters.
Reallocs a new matrix with same elements but the one at the index.
RETURN: the new matrix, NULL on malloc(3) failure, the same matrix on IOR. 
(IOR: Index Out of Range)
NOTE: the areas pointed by the elements of the matrix is left untouched,
except for the one at the index witch is free'd. */
void	**drop_index(void **mtx, int index)
{
	void	**new_mtx;
	size_t	len;
	int		i;

	len = ft_mtxlen((const void **)mtx);
	if (index < 0 || index >= (int)len)
		return (mtx);
	if (len == 1 && index == 0)
		return (free(mtx[0]), free(mtx), NULL);	//better syntex pls
	new_mtx = (void **)ft_calloc((len), sizeof(void *));
	if (new_mtx == NULL)
		return (ft_printfd(STDERR_FILENO, "malloc failure\n"), mtx);
	i = 0;
	while (mtx[i] != NULL)
	{
		if (i == index)
		{
			free(mtx[i]);
			mtx++;
		}
		if (mtx[i] == NULL)
			break ;
		new_mtx[i] = mtx[i];
		i++;
	}
	//ft_print_charr((const char **)new_mtx);
	return (free(--mtx), new_mtx);
}

/* just a var_is_valid() wrapper. 
takes both 'var' and 'var=' as valid string. */
int	var_check(const char *var)
{
	char	my_var[VARNAME];
	int		eq;
	
	eq = ft_strichr(var, '=');
	if (eq == 1 || eq > VARNAME)
		return (0);
	//ft_printf("no = found\n");
	ft_bzero(my_var, VARNAME);
	if (eq != 0)
		ft_strlcpy(my_var, var, eq + 1);
	else
		ft_strlcpy(my_var, var, ft_varlen(var) + 1);
	if (!var_is_valid(my_var))
		return (0);
	return (1);
}

/* checks if the var is a valid name:
	no spaces (neither leading nor trailing)
	only alphabetical chars (upper or lowe)
	only separator allowed: '_'
RETURNS: 1 if the var is valid, 0 if it isn't.
NOTE: you should throw a 'not a valid identifier' error (Exit Code: 1). */
int	var_is_valid(const char *var)
{
	size_t	i;

	if (var == NULL)
		return (0);
	//ft_printf("validating: '%s'\n", var);
	i = 0;
	while (var[i] != '\0')
	{
		if (i == 0 && !ft_isalpha(var[i]))
			return (0);
		if (!ft_isalpha(var[i]) && !ft_isdigit(var[i]) && var[i] == '_')
			return (0);
		i++;
	}
	return (1);
}

/*
var1=123
var2=laskd
var3=
var4=123;12k3
NOTE: target: 'var' without the '=' */
char	*get_value(const char *target, const char **mtx)
{
	size_t	tar_len;
	int		index;
	char	*my_tar;
	
	if (target == NULL || mtx == NULL)
		return (NULL);
	tar_len = ft_varlen(target);			//just for flexible usage
	my_tar = ft_substr(target, 0, tar_len); 	//VARLEN DISCUSSION
	//ft_printf("looking for '%s' -> '%s' in vars[?]\n", target, my_tar);
	if (!var_check(target))
	{
		//throw error? (treated as a cmd (or a path if contains '/'))
		free(my_tar);
		return (NULL);
	}
	index = is_there(mtx, my_tar);
	if (index >= 0)
	{
		//ft_printf("found '%s' in vars[?]\n", mtx[index]);
		free(my_tar);
		return(ft_substr(mtx[index], tar_len + 1, ft_strlen(mtx[index] + tar_len + 1)));
	}
	free(my_tar);
	return (NULL);
}


/* sets the value of the var pointed by dest,
to the int passed by value
EXPECTED: "VARNAME=VALUE" string pointed by *dest.
NOTE: we need some allocated space after the '=', len indicates that
RETURNS: 1 on successfull execution, 0 on error.
NOTE2: as var_append(), also setnum() is safe to assign to the original matrix
since it preserve the original value even on malloc failures. */
char	**setnum(char **mtx, const char *target, int value)
{
	char	*str_val;
	char	new_var[23];	// max value = 9999999999(10 cifre), min value = -999999999(9 cifre)

	if (!target)
		return (mtx);
	str_val = (char *)ft_itoa(value);
	if (str_val == NULL)
	{
		ft_printfd(STDERR_FILENO, "minishell: malloc: Allocation failure (hopefully handled)\n");
		return (mtx);
	}
	ft_bzero(new_var, 23);
	ft_strlcpy(new_var, target, ft_strlen(target) + 1);
	ft_strlcat(new_var, "=", ft_strlen(new_var) + 2);
	ft_strlcat(new_var, str_val, ft_strlen(new_var) + ft_strlen(str_val) + 1);
	//ft_printf("var_append(ing): '%s'\n", new_var);
	mtx = var_append(mtx, new_var);
	free(str_val);
	return (mtx);
}

/* RETURNS: 1 on good execution, 0 on error */
/* int	change_value(char **dest, char *src)
{
	char	*new_dest;
	int		src_len;
	int		eq;

	if (!dest || !*dest)
		return (0);
	//free(*dest);
	eq = ft_strichr(*dest, '=');
	src_len = ft_strlen(src);
	new_dest = ft_realloc(*dest, eq * sizeof(char), (eq + src_len) * sizeof(char));
	if (new_dest == NULL)
		return (NULL);
	ft_memcpy(*dest)
} */

/* we're expecting to find an mtx_var with same key as var at the index 'index',
so we free that string and replace it with var.
RETURNS: a pointer to the matrix, NULL on malloc failure or NULL mtx. */
static char	**replace_var(int index, char **mtx, char *var)
{
	if (mtx == NULL)
		return (NULL);
	free(mtx[index]);
	mtx[index] = ft_strdup(var);
	if (mtx[index] == NULL)
	{
		ft_printfd(2, "malloc failre\n");
		//ft_freentf("2", mtx);
		return (NULL);
	}
//	ft_print_charr((const char **)mtx);
	return (mtx);
}

/* adds a new var_string allocated with malloc(3) to the passed matrix.
If the var_string is NULL the var_append just returns the ptr to the matrix. 
var_string syntax: varname=varvalue.
NOTE: the var_string must be a NULL-terminated string.
RETURNS: a pointer to the head of the matrix, NULL if a malloc fails.
NOTE: is safe to assign the originale matrix to the return of var_append,
since if it fails somehow just returns the old matrix.
NOTE2: a valid var is expected, var_is_valid() function check. */
char	**var_append(char **mtx, char *var)
{
	char	**new_matrix;
	size_t	len;
	int		index;

//	ft_print_charr((const char **)mtx);
	if (var == NULL)
		return (mtx);
	len = ft_mtxlen((const void **)mtx);
	index = is_there((const char **)mtx, var);
	if (index >= 0)
		return (replace_var(index, mtx, var));
	new_matrix = ft_realloc(mtx, len * sizeof(char *), (len + 2) * sizeof(char *));
	if (new_matrix != NULL)
	{
		new_matrix[len] = ft_strdup(var);
		if (new_matrix[len] != NULL)
		{
			free(mtx);
			new_matrix[len + 1] = NULL;
			//ft_print_charr((const char **)new_matrix);
			return (new_matrix);
		}
		free(new_matrix);
	}
	//not freeing so the arr is still usable
	ft_printfd(STDERR_FILENO, "minishell: malloc error that maybe is handled\n");
	return (mtx);
}

/* initialize the terminal.
RETURNS: 1 if all good, 0 on error. */
int	term_init(void)
{	
	// Initialize the terminal
    char *termtype = getenv("TERM");
    if (termtype == NULL)
	{
        ft_printfd(STDERR_FILENO, "No TERM environment variable set\n");
        return (0);
    }

    // Set up terminfo database and check if it's available
    int result = tgetent(NULL, termtype);
    if (result == -1)
	{
        ft_printfd(STDERR_FILENO, "Could not access the terminfo database\n");
        return (0);
    }
	else if (result == 0)
	{
        ft_printfd(STDERR_FILENO, "Terminal type '%s' is not defined in terminfo\n", termtype);
        return (0);
    }
	return (1);
}

/* clears the terminal.
RETURNS: 1 if all good, 0 on error. */
int	term_clear(void)
{
	// Get the 'clear' string capability from the terminal's terminfo
    char *clear_str = tgetstr("cl", NULL);
    if (clear_str == NULL)
	{
        ft_printfd(STDERR_FILENO, "Clear capability is not available\n");
        return (0);
    }

    // Use tputs to send the escape sequence to clear the screen
    tputs(clear_str, 1, ft_putchar);
	return (1);
}
