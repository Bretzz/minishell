/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_wild_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 12:34:35 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/30 20:34:55 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

char	*get_wild_value(size_t *i, char *str);

static char	*rebuild_string(char **split, int c)
{
	unsigned int	i;
	char			*str;

	(void)c;
	str = NULL;
	i = 0;
	while (split[i] != NULL)
	{
		str = ft_strjoin_free_space(str, split[i]);
		i++;
	}
	return (str);
}

static char	*sort_words(char *str)
{
	char		**split;
	char		all_done;
	char		*sort_str;
	int			i;

	split = ft_split(str, ' ');
	if (split == NULL)
		return (str);
	all_done = 0;
	i = 0;
	while (split[i] != NULL)
	{
		if (split[i + 1] && ft_strcmp_nocase(split[i], split[i + 1]) > 0
			&& ++all_done)
			swap_ptr((void **)&split[i], (void **)&split[i + 1]);
		else if (split[i + 1] == NULL && all_done != 0)
		{
			all_done = 0;
			i = -1;
		}
		i++;
	}
	sort_str = rebuild_string(split, ' ');
	free_mtx((void **)split);
	return (sort_str);
}

static char	*build_wild_value(DIR *dir, char *pre, char *post)
{
	struct dirent	*entry;
	char			*exp_wild;
	unsigned int	i;

	exp_wild = NULL;
	i = 0;
	while (!0)
	{
		entry = readdir(dir);
		if (entry == NULL)
			break ;
		if (entry->d_name[0] != '.')
		{
			if ((!pre || !ft_strncmp(pre, entry->d_name, ft_strlen(pre)))
				&& (!post || !ft_strbcmp(post, entry->d_name, ft_strlen(post))))
			{
				exp_wild = ft_strjoin_free_space(exp_wild, entry->d_name);
				i++;
			}
			if (i != 0 && exp_wild == NULL)
				break ;
		}
	}
	return (exp_wild);
}

static char	*get_pre_wild(char *str, size_t i)
{
	size_t	n;

	if (i == 0 || ft_isspace(str[i - 1]))
		return (NULL);
	n = i - 1;
	while (n && !ft_isspace(str[n]))
		n--;
	return (ft_substr(str, n, i - n));
}

/* returns the wildcard expanded and 'covers' leading chars with
the '*'. NOTE: $USER* expands to topiana-*. */
char	*get_wild_value(size_t *i, char *str)
{
	DIR		*dir;
	char	*wild_value;
	char	*sort_value;
	char	*pre;
	char	*post;

	pre = NULL;
	post = NULL;
	dir = opendir(".");
	if (dir == NULL)
		return (write(STDERR_FILENO, "minishell: \
currdir: Permission denied\n", 38), NULL);
	if (str[*i + 1] && !ft_isspace(str[*i + 1]))
		post = ft_substr(str, *i + 1, ft_strlen_space(&str[*i + 1]));
	if (*i != 0 && !ft_isspace(str[*i - 1]))
		pre = get_pre_wild(str, *i);
	wild_value = build_wild_value(dir, pre, post);
	sort_value = sort_words(wild_value);
	if (pre != NULL)
	{
		ft_memmove(&str[*i - ft_strlen(pre)],
			&str[*i], ft_strlen(&str[*i]) + 1);
		*i -= ft_strlen(pre);
	}
	closedir(dir);
	return (free(pre), free(post), free(wild_value), sort_value);
}

// int main(void)
// {
// 	size_t	i;
// 	char	*str = ft_strdup("*.c");
// 	char	*wild;

// 	if (!0)
// 		ft_printf("im 1!\n");
// 	i = 0;
// 	wild = get_wild_value(i, str);
// 	ft_printf("wild:\n%s\n", wild);
// 	free(wild);
// 	free(str);
// 	return (0);
// }
