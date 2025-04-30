/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   weird_strlen.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 21:58:13 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/30 16:15:25 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	ft_varlen(const char *s);
size_t	ft_mtxlen(const void **mtx);
size_t	ft_strlen_nl(const char *s);
size_t	ft_strlen_space(const char *s);
size_t	ft_int_nonzero_len(const int *arr);

size_t	ft_int_nonzero_len(const int *arr)
{
	size_t	i;

	i = 0;
	while (arr[i] != 0)
		i++;
	return (i);
}

/* both ab123 and $ab123 returns 5 as length. */
size_t	ft_varlen(const char *s)
{
	size_t	i;

	if (s == NULL)
		return (0);
	i = 0;
	if (s[i] == '$')
		s++;
	if (s[i] && (s[i] == '?' || s[i] == '$' || ft_isdigit(s[i])))
		return (1);
	while (s[i] && (ft_isalnum(s[i]) || s[i] == '_'))
		i++;
	return (i);
}

size_t	ft_mtxlen(const void **mtx)
{
	size_t	i;

	if (mtx == NULL)
		return (0);
	i = 0;
	while (mtx[i] != NULL)
		i++;
	return (i);
}

size_t	ft_strlen_nl(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] && s[i] != '\n')
		i++;
	return (i);
}

size_t	ft_strlen_space(const char *s)
{
	size_t	i;

	if (s == NULL)
		return (0);
	i = 0;
	while (s[i] && s[i] != ' ')
		i++;
	return (i);
}
