/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 17:46:02 by march             #+#    #+#             */
/*   Updated: 2025/04/11 14:12:01 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_export(int fd, char **exp);

/* writes the varstr on the passed fd with the 'export' syntax. */
static void	print_var_export(int fd, const char *varstr)
{
	int	eq;

	eq = ft_strichr(varstr, '=');
	if (eq != 0)
		ft_printfd(fd, "declare -x %z\"%s\"\n", varstr, eq, varstr + eq);
	else
		ft_printfd(fd, "declare -x %s\n", varstr);
}

/* takes an int array of size 'len' and a mtx-matrix 'exp'.
Cycles trough evey element of exp to find the ones with lower ascii value
(whole 'name=value' checked) and the correspondig index in 'printed' is 0.
RETUENS: the index found, -1 if the string in 'exp' with that index is NULL. */
static int	get_best_ascii(int *printed, unsigned int len, char **exp)
{
	unsigned int	best_ascii;
	unsigned int	i;

	best_ascii = 1;
	while (best_ascii < len && printed[best_ascii] != 0)
		best_ascii++;
	if (exp[best_ascii] == NULL)
		return (-1);
	i = 1;
	while (exp[i] != NULL)
	{
		if (!printed[i] && ft_strncmp(exp[i], exp[best_ascii],
				ft_strlen(exp[i])) < 0)
			best_ascii = i;
		i++;
	}
	return (best_ascii);
}

void	print_export(int fd, char **exp)
{
	int				best_ascii;
	unsigned int	len;
	int				all_done;
	int				*printed;

	len = ft_mtxlen((const void **)exp);
	printed = (int *)ft_calloc(len, sizeof(int));
	if (printed == NULL)
	{
		write(STDERR_FILENO, "minishell: malloc failure\n", 26);
		return ;
	}
	all_done = len;
	while (all_done > 0)
	{
		best_ascii = get_best_ascii(printed, len, exp);
		if (best_ascii < 0)
			break ;
		print_var_export(fd, exp[best_ascii]);
		printed[best_ascii] = 1;
		all_done--;
	}
	free(printed);
}
