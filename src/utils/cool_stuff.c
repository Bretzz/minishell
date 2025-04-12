/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cool_stuff.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 18:41:39 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/12 20:12:30 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		is_white(char *str);
char	*drop_string(char *str, int start, size_t len);

/* checks weather a line is composed of only blank spaces. */
int	is_white(char *str)
{
	size_t	i;

	if (str == NULL)
		return (1);
	i = 0;
	while (str[i] && ft_isspace(str[i]))
		i++;
	if (str[i] == '\0')
		return (1);
	return (0);
}


/* Removes 'len' bytes from the string starting from start */
char	*drop_string(char *str, int start, size_t len)
{
	char	*new_str;
	char	*tar_ptr;
	int		i;

	if (str == NULL)
		return (NULL);
	if ((size_t)start > ft_strlen(str))
		return (ft_strdup(str));
	else if (start + len > ft_strlen(str))
		len = ft_strlen(str) - start;
	tar_ptr = &str[start];
	new_str = (char *)ft_calloc(ft_strlen(str) - len + 1, sizeof(char));
	if (new_str == NULL)
		return (NULL);
	i = 0;
	while (str[i] != '\0')
	{
		if (&str[i] != tar_ptr)
		{
			new_str[i] = str[i];
			i++;
		}
		else
			str += len;
	}
	return (new_str);
}
