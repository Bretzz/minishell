/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 13:56:38 by mapascal          #+#    #+#             */
/*   Updated: 2025/04/15 23:27:08 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

static void	first_print(t_token *tokens)
{
	if (tokens->type == TOKEN_WORD)
		ft_printf("WORD (%s)", tokens->value);
	else if (tokens->type == TOKEN_PIPE)
		ft_printf("PIPE (|)");
	else if (tokens->type == TOKEN_RED_INPUT)
		ft_printf("REDIRECT_IN (<)");
	else if (tokens->type == TOKEN_RED_OUTPUT)
		ft_printf("REDIRECT_OUT (>)");
	else if (tokens->type == TOKEN_HERE_DOC)
		ft_printf("HERE_DOC (<<)");
	else if (tokens->type == TOKEN_APPEND)
		ft_printf("APPEND (>>)");
	else if (tokens->type == TOKEN_SEMICOL)
		ft_printf("SEMICOLON (;)");
	else if (tokens->type == TOKEN_S_QUOTE)
		ft_printf("SINGLE QUOTE (')");
	else if (tokens->type == TOKEN_D_QUOTE)
		ft_printf("DOUBLE QUOTE (\")");
}

static void	second_print(t_token *tokens)
{
	if (tokens->type == TOKEN_AND_OP)
		ft_printf("'AND' OPERATOR (&&)");
	else if (tokens->type == TOKEN_OR_OP)
		ft_printf("'OR' OPERATOR (||)");
	else if (tokens->type == TOKEN_OPEN_BR)
		ft_printf("OPEN BRAKETS ('(')");
	else if (tokens->type == TOKEN_CLOSE_BR)
		ft_printf("CLOSE BRAKETS (')')");
}

void	print_tokens(t_token *tokens)
{
	while (tokens)
	{
		ft_printf("Token: ");
		first_print(tokens);
		second_print(tokens);
		ft_printf("\n");
		tokens = tokens->next;
	}
}

void	print_cmd_array(t_cmd *cmd_array, size_t num_cmds)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (i < num_cmds)
	{
		ft_printf("Comando %d:\n", i);
		j = 0;
		while (cmd_array[i].words[j] != NULL)
		{
			ft_printf("  arg[%d]     : %s\n", j, cmd_array[i].words[j]);
			j++;
		}
		ft_printf("  fd         : [%i, %i]\n", cmd_array[i].fd[0],
			cmd_array[i].fd[1]);
		ft_printf("  infile     : %s\n", cmd_array[i].infile);
		ft_printf("  outfile    : %s\n", cmd_array[i].outfile);
		ft_printf("  append flag: %d\n", cmd_array[i].append);
		ft_printf("  redir      : [%i, %i]\n", cmd_array[i].redir[0],
			cmd_array[i].redir[1]);
		i++;
	}
}
