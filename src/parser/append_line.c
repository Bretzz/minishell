/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   append_line.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 20:19:02 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/15 19:46:16 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

char	*append_line(char *line, t_token_type last_type, char ***vars);

static void	cleanup(char *line, char quote, char ***vars)
{
	add_history(line);
	free(line);
	if (g_last_sig == SIGINT)
	{
		mtx_setdata(128 + g_last_sig, vars[0], 1);
		g_last_sig = 0;
		return ;
	}
	if (quote != 0)
		ft_printfd(STDERR_FILENO, "minishell: \
syntax error near unexpected EOF while looking for matching `%c'\n", quote);
	else
		ft_printfd(STDERR_FILENO, "minishell: \
syntax error near unexpected EOF\n");
	mtx_setdata(2, vars[0], 1);
	return ;
}

static char	*get_black_line(int fd)
{
	char	*line;

	write(STDOUT_FILENO, "> ", 2);
	line = trim_back_nl(get_safe_line(fd));
	while (line && is_white(line))
	{
		free(line);
		write(STDOUT_FILENO, "> ", 2);
		line = trim_back_nl(get_safe_line(fd));
	}
	return (line);
}

static char	*pipe_join(char *line, char quote, char ***vars)
{
	char	*next_line;

	next_line = get_black_line(STDIN_FILENO);
	if (next_line == NULL)
		return (cleanup(line, quote, vars), NULL);
	line = ft_strjoin_free_space(line, next_line);
	free(next_line);
	return (line);
}

static char	*word_join(char *line, char quote, char ***vars)
{
	char	*next_line;

	write(STDOUT_FILENO, "> ", 2);
	next_line = get_safe_line(STDIN_FILENO);
	if (next_line == NULL)
		return (cleanup(line, quote, vars), NULL);
	line = ft_strjoin_free_nl(line, next_line);
	free(next_line);
	return (line);
}

char	*append_line(char *line, t_token_type last_type, char ***vars)
{
	if (*((unsigned char *)vars[0] + 6) == 1)
		input_initializer();
	if (last_type == TOKEN_PIPE)
		line = pipe_join(line, '\0', vars);
	else if (last_type == TOKEN_WORD)
		line = word_join(line, get_last_quote(line), vars);
	if (*((unsigned char *)vars[0] + 6) == 1)
		idle_initializer();
	return (line);
}
