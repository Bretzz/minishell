/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cool_stuff.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 18:41:39 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/15 23:07:50 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		is_white(char *str);
int		is_number(char *str);
char	*drop_string(char *str, int start, size_t len);
char	*trim_back_nl(char *str);
int		chrnum(char *str, int c);

/* Counts how many instances of 'c' are in 'str' */
int	chrnum(char *str, int c)
{
	unsigned int	n;
	size_t			i;

	i = 0;
	n = 0;
	while (str[i] != '\0')
	{
		if (str[i] == c)
			n++;
		i++;
	}
	return (n);
}

/* removes the trailing \n if present */
char	*trim_back_nl(char *str)
{
	size_t	i;

	if (str == NULL)
		return (str);
	i = 0;
	while (str[i] != '\0')
		i++;
	if (str[i - 1] == '\n')
		str[i - 1] = '\0';
	return (str);
}

/* checks weather a line is composed of only digits. */
int	is_number(char *str)
{
	size_t	i;

	if (str == NULL)
		return (1);
	i = 0;
	while (str[i] && ft_isdigit(str[i]))
		i++;
	if (str[i] == '\0')
		return (1);
	return (0);
}

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
	else if (len + start > ft_strlen(str))
		len = ft_strlen(str) - start;
	tar_ptr = &str[start];
	new_str = (char *)ft_calloc(ft_strlen(str) - len + 1, sizeof(char));
	if (new_str == NULL)
		return (NULL);
	i = 0;
	while (str[i] != '\0')
	{
		if (&str[i] == tar_ptr)
			str += len;
		new_str[i] = str[i];
		if (str[i] == '\0')
			break ;
		i++;
	}
	return (new_str);
}
