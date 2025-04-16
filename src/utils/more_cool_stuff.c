/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   more_cool_stuff.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 14:31:28 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/16 14:33:06 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

int		ft_strbcmp(const char *s1, const char *s2, size_t n);
int		ft_strcmp_nocase(const char *s1, const char *s2);
void	swap_ptr(void **ptr1, void **ptr2);

void	swap_ptr(void **ptr1, void **ptr2)
{
	void	*tmp;

	tmp = *ptr1;
	*ptr1 = *ptr2;
	*ptr2 = tmp;
}

int	ft_strbcmp(const char *s1, const char *s2, size_t n)
{
	size_t	i[3];

	if (n == 0)
		return (0);
	i[0] = ft_strlen(s1);
	i[1] = ft_strlen(s2);
	i[2] = 0;
	while (i[0] && i[1] && s1[i[0]] == s2[i[1]] && i[2] < n)
	{
		i[0]--;
		i[1]--;
		i[2]++;
	}
	return ((unsigned char)(s1[i[0]])
		- (unsigned char)(s2[i[1]]));
}

int	ft_strcmp_nocase(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] && s2[i] && ft_toupper(s1[i]) == ft_toupper(s2[i]))
		i++;
	return ((unsigned char)(ft_toupper(s1[i]))
		- (unsigned char)(ft_toupper(s2[i])));
}
