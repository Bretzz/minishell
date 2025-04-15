/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 15:50:46 by mapascal          #+#    #+#             */
/*   Updated: 2025/04/15 17:27:58 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

#define MAX_CMDS 100

/* 
   parse_tokens:
   - Ottiene la lista di token dalla funzione tokenizer().
   - Converte la lista di token in un array statico di comandi (t_cmd).
   - Per ogni token:
		* Se è un TOKEN_WORD, lo aggiunge all'array degli argomenti 
			del comando corrente.
		* Se è un operatore di redirezione (<, >, >>), salta al token successivo
			(assumendo che sia il nome del file) e 
			copia il nome nel campo infile o outfile.
		* Se è TOKEN_PIPE o TOKEN_SEMICOL, "chiude" il comando corrente
			e lo aggiunge all'array.
   - Infine, aggiunge l'ultimo comando e restituisce l'array.
*/

/* RETURNS: 1 on successful execution, 0 on malloc failure. */
static int	process_word(t_token *token, t_cmd *current_cmd)
{
	int	i;

	if (token->value == NULL)
		return (1);
	i = 0;
	while (current_cmd->words[i] != NULL)
		i++;
	current_cmd->words[i] = ft_strdup(token->value);
	if (!current_cmd->words[i])
	{
		write(STDERR_FILENO, "minishell: malloc failure\n", 26);
		return (0);
	}
	return (1);
}

/* Takes a token list and the vars as parameter.
Expands every TOKEN_WORD's value with env or shell variables inside,
then removes quotes.
RETURNS: 0 on malloc failure, 1 on successful expansion. */
static int	expand_tokens(t_token *tokens, const char ***vars)
{
	char	*exp_value;
	t_token	*prev;

	prev = NULL;
	while (tokens)
	{
		if (tokens->type == TOKEN_WORD
			&& (!prev || prev->type != TOKEN_HERE_DOC))
		{
			exp_value = expand_string(tokens->value, vars);
			if (exp_value == NULL)
			{
				write(STDERR_FILENO, "minishell: malloc failure\n", 26);
				return (0);
			}
			tokens->not_expanded = tokens->value;
			tokens->value = exp_value;
		}
		prev = tokens;
		tokens = tokens->next;
	}
	return (1);
}

void	*kepler(t_token **tokens, int *cmd_index,
	t_cmd *cmd_array, const char ***vars)
{
	if ((*tokens)->type == TOKEN_WORD)
	{
		if (!process_word((*tokens), &cmd_array[*cmd_index]))
			return (free_cmd(cmd_array), NULL);
	}
	else if ((*tokens)->type == TOKEN_RED_INPUT
		|| (*tokens)->type == TOKEN_RED_OUTPUT
		|| (*tokens)->type == TOKEN_APPEND
		|| (*tokens)->type == TOKEN_HERE_DOC)
		process_redirection(tokens, &cmd_array[*cmd_index], vars);
	else if ((*tokens)->type == TOKEN_PIPE || (*tokens)->type == TOKEN_SEMICOL)
	{
		if ((cmd_array[*cmd_index]).redir[1] == STDL)
			(cmd_array[*cmd_index]).redir[1] = PIPE;
		if ((cmd_array[*cmd_index]).words[0] != NULL)
			(*cmd_index)++;
		else
			multicose((cmd_array[*cmd_index]).fd);
		ft_bzero(&cmd_array[*cmd_index], sizeof(t_cmd));
		(cmd_array[*cmd_index]).redir[0] = PIPE;
	}
	return (cmd_array);
}

void	*commander(t_token *tokens, int	*cmd_index, const char ***vars)
{
	t_cmd	*cmd_array;

	cmd_array = ft_calloc(sizeof(t_cmd), cmds_count(tokens));
	if (!cmd_array)
		return (write(STDERR_FILENO, "minishell: malloc failure\n", 26), NULL);
	while (tokens)
	{
		if (g_last_sig != 0 || !(kepler(&tokens, cmd_index, cmd_array, vars)))
			break ;
		tokens = tokens->next;
	}
	return (cmd_array);
}

t_cmd	*parse_tokens(char *line, const char ***vars)
{
	t_token	*tokens;
	t_cmd	*cmd_array;
	t_cmd	current_cmd;
	int		cmd_index;

	tokens = tokenizer(line);
	if (!expand_tokens(tokens, vars))
		return (NULL);
	cmd_array = NULL;
	cmd_index = 0;
	ft_bzero(&current_cmd, sizeof(t_cmd));
	cmd_array = commander(tokens, &cmd_index, vars);
	if (cmd_array == NULL)
	{
		free_tokens(tokens);
		return (NULL);
	}
	free_tokens(tokens);
	if (current_cmd.words[0] == NULL)
		raccattagarbage(current_cmd);
	cmd_index++;
	return (cmd_array);
}
