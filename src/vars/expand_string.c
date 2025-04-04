/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_string.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 19:25:47 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/04 22:32:04 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *expand_string(char *str, const char ***vars);
char *just_expand_string(char *str, const char ***vars);

static char	*cut_string(char *str, int tar_index)
{
	char	*new_str;
	char	*tar_ptr;
	int		tar_len;
	int		i;

	if (str == NULL)
		return (NULL);
	tar_len = ft_varlen(&str[tar_index]) + 1;
	tar_ptr = &str[tar_index];
	new_str = (char *)malloc(ft_strlen(str) - tar_len + 1);
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
			str += tar_len;
	}
	new_str[i] = '\0';
	return (new_str);
}

/* searches all the vars arrays for the var pointer by str */
static char	*wide_search(char *str, const char ***vars)
{
	char	*value;
	char	*name;
	int		i;
	
	name = ft_substr(str, 0, ft_varlen(str));
	if (!name)
		return (NULL);
	//ft_printf("searching: '%s'\n", name);
	i = 1;
	while (i >= 0)
	{
		value = mtx_findval(name, NULL, 0, (char **)(vars[i]));
		if (value != NULL)
		{
			//ft_printf("found: %s\n", value);
			free(name);
			return (value);
		}
		i--;
	}
	free(name);
	return (NULL);
}


/* takes a string (to be free'd) and the index of the var found.
Allocates with malloc(3) a new string that's a copy of the imput string
with the var_string '$var' replaced with it's value found in shv, exp or env,
then frees the input string.
RETURNS: A pointer to the new string, NULL on malloc failures. */
/* static  */char	*single_expand(size_t *i, char *str, const char ***vars)
{
	char	*exp_val;
	char	*new_str;
	int		var_len;

	if (!ft_strncmp("$?", &str[*i], 2)) // to be fixed $ab?, or $?ab
	{
		exp_val = ft_itoa(*((unsigned int *)vars[0] + 1));
		ft_printf("I CAN'T BE EXPANDED DURING PARSING!!!\nI NEED TO GET THE EXIT STATUS OF THE LAST FOREGROUND PIPE!!!\n");
	}
	else
		exp_val = wide_search(&str[*i + 1], vars);
	if (exp_val == NULL)
	{
		new_str = cut_string(str, *i);
		return (free(str), new_str);
	}
	var_len = ft_varlen(&str[*i]) + 1;
	//ft_printf("varlen=%d\n", var_len);
	new_str = (char *)malloc(ft_strlen(str) - var_len + ft_strlen(exp_val) + 1);
	if (new_str == NULL)
		return (free(exp_val), NULL);
	ft_strlcpy(new_str, str, *i + 1);
	ft_strlcat(new_str, exp_val, ft_strlen(new_str) + ft_strlen(exp_val) + 1);
	ft_strlcat(new_str, &str[*i + var_len], ft_strlen(new_str) + ft_strlen(&str[*i + var_len]) + 1);
	ft_printf("skipping %d chars\n", ft_strlen(exp_val));
	(*i) += ft_strlen(exp_val) - 1;
	return (free(exp_val), free(str), new_str);
}

static char	skip_quotes(char *str, size_t *i, char quote)
{
	if (str[*i] == '"' || str[*i] == '\'')
	{
		if (quote == 0)
		{
			quote = str[*i];
			// ft_printf("before memmove: (%p) ==%s==\n", str, str);
			ft_memmove(&str[*i], &str[*i + 1], ft_strlen(&str[*i + 1]) + 1);
			// ft_printf(" after memmove: (%p) ==%s==\n", str, str);
			//(*i)++;
			quote = skip_quotes(str, i, quote);
		}
		else if (quote == str[*i])
		{
			quote = 0;
			// ft_printf("before memmove: ==%s==\n", str);
			ft_memmove(&str[*i], &str[*i + 1], ft_strlen(&str[*i + 1]) + 1);
			// ft_printf("after memmove: ==%s==\n", str);
			//(*i)++;
			quote = skip_quotes(str, i, quote);
		}
	}
	return (quote);
}

/* str is a dynamic allocated memory addres, it will be free'd later */
// static char	*remove_quotes(char *str)
// {
// 	char	quote;
// 	int		j;
// 	int		i;
// 	char	*newstr;

// 	j = 0;
// 	i = 0;
// 	quote = 0;
// 	newstr = ft_calloc(ft_strlen(str) + 1, sizeof(char));
// 	if (!newstr)
// 		return (NULL);
// 	while (str[i])
// 	{
// 		//ft_printf("'%c'...", str[i]);
// 		quote = skip_quotes(str, &i, quote);
// 		if (str[i] == '\0')
// 			break ;
// 		newstr[j++] = str[i++];
// 	}
// 	newstr[j] = '\0';
// 	return (newstr);
// }

/* takes a string, the shvars and the env vars as parameter.
returns a mallocated string with the variables preceded by '$' expanded.
RETURNS: the expanded string, NULL on malloc error. */
char *expand_string(char *str, const char ***vars)
{
	char	*my_str;
	//char	*ret_str;
	char	quote;
	size_t	i;
	//size_t	lim[2];

	(void)vars;
	my_str = ft_strdup(str);
	if (my_str == NULL)
		return (NULL);
	quote = 0;
	i = 0;
	while (my_str[i] != '\0')
	{
		// if (my_str[i] == '"' || my_str[i] == '\'')
		// {
		// 	if (quote == 0)
		// 		quote = my_str[i];
		// 	else if (quote == my_str[i])
		// 		quote = 0;
		// }
		quote = skip_quotes(my_str, &i, quote);
		// ft_printf("skip_quotes (%p) ==%s==\n", my_str, my_str);
		if (my_str[i] == '$' && quote != '\'')
			my_str = single_expand(&i, my_str, vars);
		else
			i++;
	}
	// lim[0] = 0;
	// lim[1] = ft_strlen(my_str);
	// if ((my_str[lim[0]] == '\'' || my_str[lim[0]] == '"') 
	// 	&& my_str[lim[0]] == my_str[lim[1]])
	// {
	// 	lim[0]++;
	// 	lim[1] -= 2;
	// }
	// ret_str = ft_substr(my_str, lim[0], lim[1]);
	// free(my_str);
	return (my_str);
}

/* expands every variable found */
char *just_expand_string(char *str, const char ***vars)
{
	char	*my_str;
	size_t	i;

	(void)vars;
	my_str = ft_strdup(str);
	if (my_str == NULL)
		return (NULL);
	i = 0;
	while (my_str[i] != '\0')
	{
		if (my_str[i] == '$')
			my_str = single_expand(&i, my_str, vars);
		else
			i++;
	}
	return (my_str);
}

