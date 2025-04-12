/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   append_line.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 20:19:02 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/12 20:20:16 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

char	*append_line(char *line, t_token_type last_type, char sig_flag);

static void	cleanup(char *line)
{
	add_history(line);
	free(line);
	if (g_last_sig == SIGINT)
	{
		g_last_sig = 0;
		return ;
	}
	ft_printfd(STDERR_FILENO, "minishell: syntax error near unexpected end of file\n");
	return ;
}

static char *real_line(const char *prompt)
{
	char	*real_line;
	
	real_line = /* ft_ */readline(prompt);
	while (real_line && is_white(real_line))
	{
		free(real_line);
		real_line = /* ft_ */readline(prompt);
	}
	return (real_line);
}

char	*append_line(char *line, t_token_type last_type, char sig_flag)
{
	char	*next_line;
	char	*prompt;

	prompt = "";
	if (sig_flag == 1)
	{
		prompt = "> ";
		input_initializer();
	}
	if (last_type == TOKEN_PIPE)
	{
		next_line = real_line(prompt);
		if (next_line == NULL)
			return (cleanup(line), NULL);
		line = ft_strjoin_free_space(line, next_line);
		free(next_line);
	}
	else if (last_type == TOKEN_WORD)
	{
		next_line = /* ft_ */readline(prompt);
		if (next_line == NULL)
			return (cleanup(line), NULL);
		line = ft_strjoin_free_nl(line, next_line);
		free(next_line);
	}
	if (sig_flag == 1)
		idle_initializer();
	return (line);
}
