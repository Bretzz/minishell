/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirectors.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 21:27:20 by mapascal          #+#    #+#             */
/*   Updated: 2025/04/15 14:35:43 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

static int	error_check(char *filename, char *not_expanded, char second_flag)
{
	if (filename[0] == '\0')
	{
		ft_perror(not_expanded, "ambiguous redirect", NULL, 1);
		return(1);
	}
	else if (ft_strlen(filename) > MAX_PATH)
	{
		ft_perror(filename, "File name too long", NULL, 1);
		return(1);
	}
	else if (access(filename, F_OK) != 0 && second_flag == R_OK)
	{
		ft_perror(filename, "No such file or directory", NULL, 1);
		return(1);
	}
	else if (access(filename, second_flag) != 0)
	{
		ft_perror(filename, "Permission denied", NULL, 1);
		return(1);
	}
	return (0);
}

static void	redir_trunc_open(t_cmd *current_cmd, char *filename,
	char *not_expanded)
{
	if (current_cmd->fd[1] < 0)
		return ;
	safeclose(current_cmd->fd[1]);
	ft_strlcpy(current_cmd->outfile, filename, MAX_PATH + 1);
	current_cmd->redir[1] = FILE;
	if (error_check(filename, not_expanded, W_OK) != 0)
	{
		current_cmd->fd[1] = -1;
		current_cmd->parse_code = EXIT_FAILURE;
		return ;
	}
	current_cmd->append = O_WRONLY | O_CREAT | O_TRUNC;
}

static void	redir_append_open(t_cmd *current_cmd, char *filename,
	char *not_expanded)
{
	if (current_cmd->fd[1] < 0)
		return ;
	safeclose(current_cmd->fd[1]);
	ft_strlcpy(current_cmd->outfile, filename, MAX_PATH + 1);
	current_cmd->redir[1] = FILE;
	if (error_check(filename, not_expanded, W_OK) != 0)
	{
		current_cmd->fd[1] = -1;
		current_cmd->parse_code = EXIT_FAILURE;
		return ;
	}
	current_cmd->append = O_WRONLY | O_CREAT | O_APPEND;
}

static void	redir_input_open(t_cmd *current_cmd, char *filename,
	char *not_expanded)
{
	if (current_cmd->fd[0] < 0)
		return ;
	safeclose(current_cmd->fd[0]);
	ft_strlcpy(current_cmd->infile, filename, MAX_PATH + 1);
	current_cmd->redir[0] = FILE;
	if (error_check(filename, not_expanded, R_OK) != 0)
	{
		current_cmd->fd[0] = -1;
		current_cmd->parse_code = EXIT_FAILURE;
		return ;
	}
	current_cmd->fd[0] = open(filename, O_RDONLY);
}

void	process_redirection(t_token **tokens, t_cmd *current_cmd,
	const char ***vars)
{
	t_token_type	redir_type;

	if (*tokens && (*tokens)->next && ((*tokens)->next->type == TOKEN_WORD))
	{
		redir_type = (*tokens)->type;
		if (redir_type == TOKEN_RED_INPUT)
			redir_input_open(current_cmd, (*tokens)->next->value,
				(*tokens)->next->not_expanded);
		else if (redir_type == TOKEN_HERE_DOC)
		{
			safeclose(current_cmd->fd[0]);
			current_cmd->fd[0] = here_doc((*tokens)->next->value, vars);
			current_cmd->redir[0] = HERE_DOC;
		}
		else if (redir_type == TOKEN_RED_OUTPUT)
			redir_trunc_open(current_cmd, (*tokens)->next->value,
				(*tokens)->next->not_expanded);
		else if (redir_type == TOKEN_APPEND)
			redir_append_open(current_cmd, (*tokens)->next->value,
				(*tokens)->next->not_expanded);
	}
	if (*tokens)
		*tokens = (*tokens)->next;
}
