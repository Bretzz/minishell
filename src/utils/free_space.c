/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_space.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 22:53:40 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/16 13:10:36 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strjoin_free_space(char *s1, char *s2);
char	*ft_strjoin_free_nl(char *s1, char *s2);
char	*ft_strjoin_free_nl_space(char *s1, char *s2);

char	*ft_strjoin_free_space(char *s1, char *s2)
{
	char	*twelve;
	size_t	size;

	if (s2 == NULL)
		return (s1);
	size = ft_strlen(s1) + ft_strlen(s2) + 2;
	twelve = (char *)ft_calloc(size, sizeof(char));
	if (twelve == NULL)
		return (free(s1), NULL);
	if (s1 != NULL)
	{
		ft_strlcpy(twelve, s1, size);
		ft_strlcat(twelve, " ", size);
	}
	if (s2 != NULL)
		ft_strlcat(twelve, s2, size);
	return (free(s1), twelve);
}

char	*ft_strjoin_free_nl(char *s1, char *s2)
{
	char	*twelve;
	size_t	size;

	if (s2 == NULL)
		return (s1);
	size = ft_strlen(s1) + ft_strlen(s2) + 2;
	twelve = (char *)ft_calloc(size, sizeof(char));
	if (twelve == NULL)
		return (free(s1), NULL);
	if (s1 != NULL)
	{
		ft_strlcpy(twelve, s1, size);
		ft_strlcat(twelve, "\n", size);
	}
	if (s2 != NULL)
		ft_strlcat(twelve, s2, size);
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
	size = ft_strlen(s1) + ft_strlen_space(s2) + 2;
	twelve = (char *)malloc(size * sizeof(char));
	if (twelve == NULL)
		return (free(s1), NULL);
	i = -1;
	while (s1[++i])
		twelve[i] = s1[i];
	j = 0;
	while (s2[j] && s2[j] != ' ')
		twelve[i++] = s2[j++];
	twelve[i] = '\n';
	twelve[i + 1] = '\0';
	return (free(s1), twelve);
}
