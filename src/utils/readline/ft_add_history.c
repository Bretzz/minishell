/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_add_history.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 18:25:23 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/09 15:07:49 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_readline.h"
#include "libft.h"

char	**ft_add_history(char *line);
char	**ft_get_history(void);
//void	ft_set_history(char **history);
void	ft_free_history(void);

/* The first call ever is an assignment call.
The pointer passed to the function will be treated
as the first entry of the history.
Successive calls will add new entries to the history
up to MAX_HISTORY enries.
IF line is NULL returns the history pointer. */
char	**ft_add_history(char *line)
{
	static char		*history[MAX_HISTORY];
	static int		index;
	unsigned int	i;
	
	if (line == NULL)
	{
		return (history);	//dunno
	}
	if (index == 0)
	{
		index++;
	}
	if (index < MAX_HISTORY)
	{
		i = index - 1;
		while (i > 0)
		{
			history[i + 1] = history[i];
			i--;
		}
		history[1] = ft_strdup(line);
		index++;
	}
	return (history);
}

/* The first call ever is an assignment call.
The pointer passed to the function will be treated
as the first entry of the history.
RETURNS: the history pointer. */
/* DOESN'T LOOK GOOD >:(
alternative: char **ft_add_history(char *) */
char	**ft_get_history(void)
{
	static char	*history[MAX_HISTORY];
	
	return (history);
}

/* Simple setter. */
// void	ft_set_history(char **history)
// {
// 	ft_add_history((char *)history);
// 	ft_get_history((char *)history);
// }

void	ft_free_history(void)
{
	char	**history;
	int		i;

	history = ft_add_history(NULL);
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
