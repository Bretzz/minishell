/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 15:58:00 by topiana-          #+#    #+#             */
/*   Updated: 2025/01/21 12:42:49 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void		ft_free_arr(char **arr);
static size_t	ft_wdlen(const char *s, int c);
int				ft_wdcount(const char *s, int c);

static void	ft_free_arr(char **arr)
{
	unsigned int	i;

	i = 0;
	while (arr[i] != NULL)
	{
		free(arr[i]);
		i++;
	}
}

static size_t	ft_wdlen(const char *s, int c)
{
	size_t	i;

	i = 0;
	while (s[i] && s[i] != c)
		i++;
	return (i);
}

int	ft_wdcount(const char *s, int c)
{
	int	wd;
	int	i;

	wd = 0;
	i = 0;
	while (s[i] != '\0')
	{
		if ((i == 0 && s[i] != c)
			|| (s[i - 1] == c && s[i] != c && s[i] >= 32))
			wd++;
		i++;
	}
	return (wd);
}

char	**ft_split(char const *s, char c)
{
	int		wd;
	int		ii;
	int		i;
	char	**mar_rosso;

	if (s == NULL)
		return (NULL);
	wd = ft_wdcount(s, c);
	mar_rosso = (char **)ft_calloc(wd + 1, sizeof(char *));
	if (mar_rosso == NULL)
		return (NULL);
	ii = 0;
	i = -1;
	while (ii < wd)
	{
		if (s[++i] != c)
		{
			mar_rosso[ii] = ft_substr(s, i, ft_wdlen(&s[i], c));
			if (mar_rosso[ii] == NULL)
				return (ft_free_arr(mar_rosso), NULL);
			i += ft_wdlen(&s[i], c);
			ii++;
		}
	}
	return (mar_rosso);
}

/* #include <stdio.h>
#include <stdlib.h>
#include <string.h>
int	main(int argc, char *argv[])
{
	char	**arr;
	unsigned int	i;
	char *splitme = strdup("Julia -1 1 1 1");

	//arr = ft_split(argv[1], *argv[2]);
	printf("%s\n", splitme);
	arr = ft_split(splitme, ' ');
	if (arr[1] == NULL)
		printf("18.OK\n");
	//printf("%s\n", arr[1]);
	i = 0;
	while (arr[i] != 0)
	{
		printf("arr[%i]='%s'\n", i, arr[i]);
		i++;
	}
	ft_free_arr(arr); free(splitme);
	return (0);
}  */
