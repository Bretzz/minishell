/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapascal <mapascal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 15:50:46 by mapascal          #+#    #+#             */
/*   Updated: 2025/03/11 15:22:50 by mapascal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

s_cmd cmd;

void	*parser(char *str)
{
	while(str[i])
	{
		if(str[i] == '<')
			red_input()
		else if(str[i] == '>')
			red_output()
		else if(str[i] == '<<')
			delimeter_input()
		else if(str[i] == '>>')
			red_output_append()
		i++;
	}
}


//tokenize crea un array di tokens facendo split
//split oppure subset gestione del $

void *tokenize(char *str, char **set2)
{
	int i;
	char *set[5] = {" ", "|", "<", "<<", ">", ">>"};
	i = 0;
	
	while(str[i])
	{
		if()
	}	
	
}

void	skip_spaces(char *line, int i)
{
	
}

int	is_operator(char *line, int i)
{
	
}

void	add_token(t_list	tokens, fun(){}
{
	
}
t_list *tokenize_line(char *line)
{
	t_list  *tokens;
	int	i;

	i = 0;
	tokens = NULL;
	while (line[i])
	{
		skip_spaces(line, &i);
		if (line[i] == '\0')
			break ;
		if (is_operator(line, i))
			add_token(&tokens, get_next_operator(line, &i));
		else
			add_token(&tokens, get_next_word(line, &i));
	}
	return (tokens);
}


char *set[5] = {' ', '|', '<', '>', '\'', '\"',};

if s[i] == *set[0]
	