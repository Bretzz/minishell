/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_space.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totommi <totommi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 22:53:40 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/10 01:14:09 by totommi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strjoin_free_nl(char *s1, char *s2);

size_t	ft_strlen_space(const char *s);
size_t	ft_strlen_nl(const char *s);

size_t	ft_mtxlen(const void **mtx);
size_t	ft_varlen(const char *s);

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
	while (s[i] && (ft_isalnum(s[i]) || s[i] == '_'/*  || s[i] == '?' */))
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

static size_t	ft_strlen_temp(const char *s)
{
	size_t	i;

	if (s == NULL)
		return (0);
	i = 0;
	while (s[i])
		i++;
	return (i);
}

char	*ft_strjoin_free_space(char *s1, char *s2)
{
	int		i;
	int		j;
	char	*twelve;
	size_t	size;

	if (s1 == NULL)
	{
		s1 = (char *)malloc(1 * sizeof(char));
		if (!s1)
			return (NULL);
		s1[0] = '\0';
	}
	if (s2 == NULL)
		return (s1);
	size = ft_strlen(s1) + ft_strlen(s2) + 2;
	twelve = (char *)malloc(size * sizeof(char));
	if (twelve == NULL)
		return (NULL);
	i = -1;
	while (s1[++i])
		twelve[i] = s1[i];
	twelve[i++] = ' ';
	j = 0;
	while (s2[j] != '\0')
		twelve[i++] = s2[j++];
	twelve[i] = '\0';
	free(s1);
	return (twelve);
}

char	*ft_strjoin_free_nl(char *s1, char *s2)
{
	int		i;
	int		j;
	char	*twelve;
	size_t	size;

	if (s1 == NULL)
	{
		s1 = (char *)ft_calloc(1, sizeof(char));
		if (s1 == NULL)
			return (NULL);
	}
	if (s2 == NULL)
		return (s1);
	size = ft_strlen(s1) + ft_strlen(s2) + 2;
	twelve = (char *)malloc(size * sizeof(char));
	if (twelve == NULL)
		return (free(s1), NULL);
	i = -1;
	while (s1[++i])
		twelve[i] = s1[i];
	twelve[i++] = '\n';
	j = 0;
	while (s2[j] != '\0')
		twelve[i++] = s2[j++];
	twelve[i] = '\0';
	return (free(s1), twelve);
}

char	*ft_strjoin_free_nl_space(char *s1, char *s2)
{
	int		i;
	int		j;
	char	*twelve;
	size_t	size;

	if (s1 == NULL)
	{
		s1 = (char *)malloc(1 * sizeof(char));
		if (!s1)
			return (NULL);
		s1[0] = '\0';
	}
	//ft_printf("spaceeee\n");
	size = ft_strlen_temp(s1) + ft_strlen_space(s2) + 2;
	//ft_printf("size=%u\n", size);
	twelve = (char *)malloc(size * sizeof(char));
	if (twelve == NULL)
		return (NULL);
	i = -1;
	while (s1[++i])
		twelve[i] = s1[i];
	j = 0;
	while (s2[j] && s2[j] != ' ')
		twelve[i++] = s2[j++];
	twelve[i] = '\n';
	twelve[i + 1] = '\0';
	free(s1);
	return (twelve);
}
