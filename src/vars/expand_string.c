/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_string.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2025/04/12 18:04:26 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

char *expand_string(char *str, const char ***vars);
char *just_expand_string(char *str, const char ***vars);

// static char	expander_quote_check(char *line)
// {
// 	static char	quote;
// 	size_t	i;

// 	if (line == NULL)	//never happens
// 		return (quote);
// 	i = 0;
// 	while (line[i] != '\0')
// 	{
// 		if (quote == 0
// 			&& (line[i] == '\'' || line[i] == '"'))
// 		{
// 			quote = line[i];
// 			break ;
// 		}
// 		else if (line[i] == quote)
// 		{
// 			quote = 0;
// 			break ;
// 		}
// 		i++;
// 	}
// 	return (quote);
// }

/* Mallocates a new string with the '$var' found at 'tar_index' cut off. */
// static char	*cut_string(char *str, int tar_index)
// {
// 	char	*new_str;
// 	char	*tar_ptr;
// 	int		tar_len;
// 	int		i;

// 	if (str == NULL)
// 		return (NULL);
// 	tar_len = ft_varlen(&str[tar_index]) + 1;
// 	tar_ptr = &str[tar_index];
// 	new_str = (char *)malloc(ft_strlen(str) - tar_len + 1);
// 	if (new_str == NULL)
// 		return (NULL);
// 	i = 0;
// 	while (str[i] != '\0')
// 	{
// 		if (&str[i] != tar_ptr)
// 		{
// 			new_str[i] = str[i];
// 			i++;
// 		}
// 		else
// 			str += tar_len;
// 	}
// 	new_str[i] = '\0';
// 	return (new_str);
// }

/* searches all the vars arrays for the var pointer by str */
static char	*wide_search(char *str, const char ***vars)
{
	char	*value;
	char	*name;
	int		i;
	
	name = ft_substr(str, 0, ft_varlen(str));
	if (!name)
		return (NULL);
//	ft_printf("searching: '%s'\n", name);
	i = 1;
	while (i >= 0)
	{
		value = mtx_findval(name, NULL, 0, (char **)(vars[i]));
		if (value != NULL)
		{
//			ft_printf("found: %s\n", value);
			free(name);
			return (value);
		}
		i--;
	}
	free(name);
	return (NULL);
}

static char *set_exp_val(size_t *i, char *str, const char ***vars)
{
	if (str[*i] == '$' && (str[*i + 1] == '\0' || ft_isspace(str[*i + 1])))
		return ((*i)++, NULL);
	else if (str[*i] == '$' && ft_isdigit(str[*i + 1]))
	{
		if (str[*i + 1] == '0')
			return (ft_strdup("minishell"));
		return (NULL);
	}
	else if (!ft_strncmp("$?", &str[*i], 2))
		return (ft_itoa(*((unsigned char *)vars[0] + 1)));
	else if (!ft_strncmp("$$", &str[*i], 2))
		return (ft_itoa(*((unsigned int *)vars[1] + 1)));
	else if (str[*i] == '~') 
	{
		if (*i != 0 && !ft_isspace(str[*i - 1]))
			return ((*i)++, NULL);
		if (str[*i + 1] != '\0' && str[*i + 1] != '/' && !ft_isspace(str[*i + 1]))
			return ((*i)++, NULL);
		return (mtx_findval("HOME", NULL, MAX_PATH, (char **)vars[1]));
	}
	return(wide_search(&str[*i + 1], vars));
}

/* takes a string (to be free'd) and the index of the var found.
Allocates with malloc(3) a new string that's a copy of the imput string
with the var_string '$var' replaced with it's value found in shv, exp or env,
then frees the input string.
RETURNS: A pointer to the new string, NULL on malloc failures. */
static char	*single_expand(size_t *i, char *str, const char ***vars)
{
	char	*exp_val;
	char	*new_str;
	int		var_len;

	exp_val = set_exp_val(i, str, vars);
	if (exp_val == NULL)
	{
		if (str[*i] == '$' || str[*i] == '~')
		{
			new_str = drop_string(str, *i, ft_varlen(&str[*i]) + 1);
			return (free(str), new_str);
		}
		return (str);
	}
	var_len = ft_varlen(&str[*i]) + 1;
	new_str = (char *)malloc(ft_strlen(str) - var_len + ft_strlen(exp_val) + 1);
	if (new_str == NULL)
		return (free(exp_val), NULL);
	ft_strlcpy(new_str, str, *i + 1);
	ft_strlcat(new_str, exp_val, ft_strlen(new_str) + ft_strlen(exp_val) + 1);
	ft_strlcat(new_str, &str[*i + var_len], ft_strlen(new_str) + ft_strlen(&str[*i + var_len]) + 1);
	if (DEBUG) {ft_printf("skipping %d chars\n", ft_strlen(exp_val));}
	(*i) += ft_strlen(exp_val) - 1;
	return (free(exp_val), free(str), new_str);
}

/* If the index 'i' of the string 'str' is a quote the whole
string is shifted backwards, ex: "ciao'come'va", i=4 becomes: "ciaocome'va",
After that skip_quotes() is called recursively to handle consecutive quotes.
NOTE: The 'quote' char passed is referring to the last quote found, 0 if already
closed or none was found yet.
RETURNS: the value of the quote we are currently in, 0 if we just closed a quote
or if we didn't find one yet. */
static char	skip_quotes(char *str, size_t i, char quote)
{
	if (str[i] == '"' || str[i] == '\'')
	{
		if (quote == 0)
		{
			quote = str[i];
			ft_memmove(&str[i], &str[i + 1], ft_strlen(&str[i + 1]) + 1);
			quote = skip_quotes(str, i, quote);
		}
		else if (quote == str[i])
		{
			quote = 0;
			ft_memmove(&str[i], &str[i + 1], ft_strlen(&str[i + 1]) + 1);
			quote = skip_quotes(str, i, quote);
		}
	}
	return (quote);
}

/* takes a string, the shv-vars and the env-vars as parameter.
returns a mallocated string with the variables preceded by '$' expanded.
(quotes are also handled properly)
RETURNS: the expanded string, NULL on malloc error. */
char *expand_string(char *str, const char ***vars)
{
	char	*my_str;
	static char	quote;
	size_t	i;

	(void)vars;
	my_str = ft_strdup(str);
	if (my_str == NULL)
		return (NULL);
	i = 0;
	while (my_str && my_str[i] != '\0')
	{
		quote = skip_quotes(my_str, i, quote);
		if ((my_str[i] == '$' || my_str[i] == '~') && quote != '\'')
			my_str = single_expand(&i, my_str, vars);
		else
			i++;
	}
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

