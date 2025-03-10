/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_space.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 22:53:40 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/10 13:31:05 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static size_t	ft_strlen_space(const char *s)
{
	size_t	i;

	if (s == NULL)
		return (0);
	i = 0;
	while (s[i] && s[i] != ' ')
		i++;
	return (i);
}

char	*ft_strjoin_free_space_nl(char *s1, char *s2)
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
