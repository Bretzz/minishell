/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapascal <mapascal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 15:50:46 by mapascal          #+#    #+#             */
/*   Updated: 2025/03/17 18:36:36 by mapascal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

#define MAX_ARGS 100
#define MAX_CMDS 100
// typedef struct s_cmd
// {
//     char *words[MAX_ARGS];   // Array di argomenti (comando e parametri)
//     char infile[1024];       // File di input, se presente
//     char outfile[1024];      // File di output, se presente
//     int  append;             // 1 se è ">>",O_WRONLY | O_CREATE | O_APPEND se è ">" O_WRONLY | O_CREATE | O_TRUNC
// } t_cmd;


/* 
   La struct t_cmd è definita in minishell.h, ad esempio:
   
   typedef struct s_cmd
   {
	   char *words[MAX_ARGS];   // Array di argomenti (comando + parametri)
	   char infile[1024];       // File di input, se presente
	   char outfile[1024];      // File di output, se presente
	   int  append;             // 1 se è ">>", 0 se è ">"
   } t_cmd;
*/

/* Inizializza una struttura t_cmd, azzerando l'array degli argomenti e le stringhe di redirezione */
t_cmd create_cmd(void)
{
	t_cmd cmd;
	int i;

	i = 0;
	while (i < MAX_ARGS)
	{
		cmd.words[i] = NULL;
		i++;
	}
	cmd.infile[0] = '\0';
	cmd.outfile[0] = '\0';
	cmd.append = 0;
	return (cmd);
}

/* Aggiunge l'argomento 'arg' nella prima posizione libera dell'array words di t_cmd */
void add_arg_to_cmd(t_cmd *cmd, char *arg)
{
	int i;

	i = 0;
	while (cmd->words[i] != NULL)
		i++;
	cmd->words[i] = ft_strdup(arg);
	//ft_memmove(&cmd->words[i], arg, ft_strlen(arg));
}

/* Aggiunge il comando corrente nell'array di comandi e incrementa l'indice */
void append_cmd(t_cmd *cmd_array, int *index, t_cmd cmd)
{
	cmd_array[*index] = cmd;
	*index = *index + 1;
}

/* 
   parse_tokens:
   - Ottiene la lista di token dalla funzione tokenizer().
   - Converte la lista di token in un array statico di comandi (t_cmd).
   - Per ogni token:
		* Se è un TOKEN_WORD, lo aggiunge all'array degli argomenti del comando corrente.
		* Se è un operatore di redirezione (<, >, >>), salta al token successivo (assumendo che sia il nome del file) e copia il nome nel campo infile o outfile.
		* Se è TOKEN_PIPE o TOKEN_SEMICOL, "chiude" il comando corrente e lo aggiunge all'array.
   - Infine, aggiunge l'ultimo comando e restituisce l'array.
*/

static void process_word(t_token *token, t_cmd *current_cmd)
{
	add_arg_to_cmd(current_cmd, token->value);
}

static void process_redirection(t_token **tokens, t_cmd *current_cmd)
{
	t_token_type redirType;

	redirType = (*tokens)->type;
	/* Assumi che il token corrente sia un operatore di redirezione, 
	   quindi il token successivo deve essere il nome del file */
	if (*tokens && (*tokens)->next && ((*tokens)->next->type == TOKEN_WORD))
	{
		if (redirType == TOKEN_RED_INPUT)
			ft_strlcpy(current_cmd->infile, (*tokens)->next->value, 1024);
		else if (redirType == TOKEN_RED_OUTPUT)
		{
			ft_strlcpy(current_cmd->outfile, (*tokens)->next->value, 1024);
			current_cmd->append = O_WRONLY | O_CREAT | O_TRUNC;
		}
		else if (redirType == TOKEN_APPEND)
		{
			ft_strlcpy(current_cmd->outfile, (*tokens)->next->value, 1024);
			current_cmd->append = O_WRONLY | O_CREAT | O_APPEND;
		}
		else if (redirType == TOKEN_HERE_DOC)
			ft_strlcpy(current_cmd->infile, (*tokens)->next->value, 1024);
	}
	/* Avanza tokens di due posizioni: l'operatore e il nome del file */
	if (*tokens)
		*tokens = (*tokens)->next;
	if (*tokens)
		*tokens = (*tokens)->next;
}

// static int is_separator(t_token *token)
// {
//     if (token->type == TOKEN_PIPE || token->type == TOKEN_SEMICOL)
//         return (1);
//     return (0);
// }

t_cmd *parse_tokens(char *line)
{
	t_token *tokens;
	t_cmd *cmd_array;
	t_cmd current_cmd;
	int cmd_index;

	tokens = tokenizer(line);
	/* print_tokens(tokens);
	printf("\n\n"); */
	cmd_array = ft_calloc(sizeof(t_cmd), MAX_CMDS);
	if (!cmd_array)
		return (NULL);
	cmd_index = 0;
	current_cmd = create_cmd();
	while (tokens)
	{
		//ft_printf("looking at '%s'\n", tokens->value);
		if (tokens->type == TOKEN_WORD)
		{
			process_word(tokens, &current_cmd);
			tokens = tokens->next;
			continue;
		}
		if (tokens->type == TOKEN_RED_INPUT ||
			tokens->type == TOKEN_RED_OUTPUT ||
			tokens->type == TOKEN_APPEND)
		{
			process_redirection(&tokens, &current_cmd);
			continue;
		}
		/* Gestione dei separatori (pipe e punto e virgola) */
		if (tokens->type == TOKEN_PIPE)
		{
			/* Imposta l'outfile del comando corrente con "|" */
			{
				char *pipe_str = get_rekd(TOKEN_PIPE);
				ft_strlcpy(current_cmd.outfile, pipe_str, 1024);
				free(pipe_str);
			}
			append_cmd(cmd_array, &cmd_index, current_cmd);
			/* Crea un nuovo comando e imposta il suo infile con "|" */
			current_cmd = create_cmd();
			{
				char *pipe_str = get_rekd(TOKEN_PIPE);
				ft_strlcpy(current_cmd.infile, pipe_str, 1024);
				free(pipe_str);
			}
			tokens = tokens->next;
			continue;
		}
		else if (tokens->type == TOKEN_SEMICOL)
		{
			append_cmd(cmd_array, &cmd_index, current_cmd);
			current_cmd = create_cmd();
			tokens = tokens->next;
			continue;
		}
		tokens = tokens->next;
	}
	append_cmd(cmd_array, &cmd_index, current_cmd);
	return (cmd_array);
}

void print_cmd_array(t_cmd *cmd_array, size_t num_cmds)
{
	size_t i;
	size_t j;

	i = 0;
	while (i < num_cmds)
	{
		ft_printf("Comando %d:\n", i);
		j = 0;
		while (cmd_array[i].words[j] != NULL)
		{
			ft_printf("  arg[%d]: %s\n", j, cmd_array[i].words[j]);
			j++;
		}
		ft_printf("  infile: %s\n", cmd_array[i].infile);
		ft_printf("  outfile: %s\n", cmd_array[i].outfile);
		ft_printf("  append flag: %d\n", cmd_array[i].append);
		i++;
	}
}

int	ft_cmdlen(t_cmd *cmd_array)
{
	size_t	i;
	
	i = 0;
	if(cmd_array == NULL)
		return(0);
	while(cmd_array[i].words[0] != NULL && cmd_array[i].words[0][0] != '\0')
		i++;
	return(i);
}

// int	main(int argc, char *argv[])
// {
// 	(void)argc;
//     char *line = argv[1];
//     t_cmd *cmd_array;
//     int num_cmds;
	
//     cmd_array = parse_tokens(line);
// 	ft_printf("blblbllb\n");

// //	int num_cmds2 = ft_mtxlen((const void **)&cmd_array);
// 	num_cmds = 	ft_cmdlen(cmd_array);
// 	ft_printf("blblbllb\n");

//     //line = "echo ciao > file"; // Prova prima con ">"; poi con ">>"
//     /* Dovrai sapere quanti comandi hai processato; per test puoi 
//        fissare num_cmds ad un valore (ad es. MAX_CMDS) oppure modificare 
//        parse_tokens per restituire anche il numero di comandi */
//   //  num_cmds = atoi(argv[2])/* numero di comandi ottenuti */;
// 	printf("ft_cmdlen-->%i\n\n", num_cmds);
// 	// printf("ft_mtxlen-->%i\n\n", num_cmds2);
// 	print_cmd_array(cmd_array, num_cmds);
//     // libera la memoria e gestisci cleanup...
//     return (0);
// }
