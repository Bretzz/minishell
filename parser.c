/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapascal <mapascal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 15:50:46 by mapascal          #+#    #+#             */
/*   Updated: 2025/03/13 12:37:29 by mapascal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*parser(char *str)
{
	t_token	*tokens;
	tokens = tokenizer(str);
	while (tokens->next)
	{
		//if token.type
		tokens = tokens->next;
	}
}


int	main(int argc, char *argv[])
{
	t_token	*tokens;

	// (void)argc;
	// tokens = parser(argv[1]);
	// print_tokens(tokens);
	return (0);
}

