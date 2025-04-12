/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 20:13:57 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/12 21:28:35 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

int		is_closed(char *line);
char	get_last_quote(char *line);

/* checks weather the string passed has unclosed quotes.
RETURNS: 0 if it has unclosed quotes, 1 if it hasn't or it's NULL. */
int	is_closed(char *line)
{
	char	quote;
	char	last;
	size_t	i;

	if (line == NULL)
		return (1);
	last = 0;
	quote = 0;
	i = 0;
	while (line[i] != '\0')
	{
		if (!ft_isspace(line[i]))
			last = line[i];
		if (quote == 0
			&& (line[i] == '\'' || line[i] == '"'))
			quote = line[i];
		else if (line[i] == quote)
			quote = 0;
		i++;
	}
	if (last == '|' || quote != 0)
		return (0);
	return (1);
}

/* returns the last unclosed quote */
char	get_last_quote(char *line)
{
	char	quote;
	size_t	i;

	if (line == NULL)
		return ('\0');
	quote = 0;
	i = 0;
	while (line[i] != '\0')
	{
		if (quote == 0
			&& (line[i] == '\'' || line[i] == '"'))
			quote = line[i];
		else if (line[i] == quote)
			quote = 0;
		i++;
	}
	return (quote);
}
