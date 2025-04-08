/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_add_history.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 18:25:23 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/08 21:17:48 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_readline.h"
#include "libft.h"

void	ft_add_history(char *line);
char	**ft_get_history(char *history);
void	ft_set_history(char **history);
void	ft_free_history(void);

/* The first call ever is an assignment call.
The pointer passed to the function will be treated
as the first entry of the history.
Successive calls will add new entries to the history
up to MAX_HISTORY enries. */
void	ft_add_history(char *line)
{
	static char	**history;
	static int	index;
	
	if (line == NULL)
	{
		;	//dunno
	}
	if (index == 0)
	{
		history = (char **)line;
		index++;
	}
	else if (index < MAX_HISTORY)
	{
		history[index] = ft_strdup(line);
		index++;
	}
}

/* The first call ever is an assignment call.
The pointer passed to the function will be treated
as the first entry of the history.
RETURNS: the history pointer. */
char	**ft_get_history(char *history)
{
	static char	**storage;
	static int	index;

	if (!index && !history)
		return (NULL);
	if (index++ == 0)
		storage = (char **)history;
	return (storage);
}

/* Simple setter. */
void	ft_set_history(char **history)
{
	ft_add_history((char *)history);
	ft_get_history((char *)history);
}

void	ft_free_history(void)
{
	char	**history;
	int		i;

	history = ft_get_history(NULL);
	if (history == NULL)
		return ;
	i = 0;
	while (i < MAX_HISTORY)
	{
	//	ft_printf("freeing '%s'\n", history[i]);
		free(history[i++]);
	}
	//free(history);
}
