/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_string_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 12:19:02 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/16 15:16:29 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

char	*expand_string_bonus(char *str, const char ***vars);

static char	*set_exp_val(size_t *i, char *str, const char ***vars)
{
	if (str[*i] == '*')
		return (get_wild_value(*i, str));
	if (str[*i] == '$' && (str[*i + 1] == '\0'
			|| ft_isspace(str[*i + 1]) || str[*i + 1] == '='))
		return ((*i)++, NULL);
	else if (str[*i] == '$' && ft_isdigit(str[*i + 1]))
	{
		if (str[*i + 1] == '0')
			return (ft_strdup("minishell"));
		return (NULL);
	}
	else if (!ft_strncmp("$?", &str[*i], 2))
		return (ft_itoa(*((unsigned char *)vars[0] + 5)));
	else if (!ft_strncmp("$$", &str[*i], 2))
		return (ft_itoa(*((unsigned int *)vars[1] + 1)));
	else if (str[*i] == '~')
	{
		if (*i != 0 && !ft_isspace(str[*i - 1]))
			return ((*i)++, NULL);
		if (str[*i + 1] != '\0' && str[*i + 1] != '/'
			&& !ft_isspace(str[*i + 1]))
			return ((*i)++, NULL);
		return (mtx_findval("HOME", NULL, 0, (char **)vars[1]));
	}
	return (wide_search(&str[*i + 1], vars));
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
	if (str[*i] == '*')
		var_len = ft_strlen_space(&str[*i]) + 5;
	else
		var_len = ft_varlen(&str[*i]) + 1;
	new_str = (char *)ft_calloc(ft_strlen(str) - var_len
			+ ft_strlen(exp_val) + 1, sizeof(char));
	if (new_str == NULL)
		return (free(exp_val), NULL);
	ft_strlcpy(new_str, str, *i + 1);
	ft_printf("new_str: '%s'\n\n", new_str);
	ft_printf("exp_val: '%s'\n\n", exp_val);
	ft_strlcat(new_str, exp_val, ft_strlen(new_str) + ft_strlen(exp_val) + 1);
	ft_printf("new_str: '%s'\n\n", new_str);
	ft_strlcat(new_str, &str[*i + var_len],
	ft_strlen(new_str) + ft_strlen(&str[*i + var_len]) + 1);
	ft_printf("new_str: '%s'\n\n", new_str);
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
char	*expand_string_bonus(char *str, const char ***vars)
{
	char		*my_str;
	static char	quote;
	size_t		i;

	(void)vars;
	my_str = ft_strdup(str);
	if (my_str == NULL)
		return (NULL);
	i = 0;
	while (my_str && my_str[i] != '\0')
	{
		quote = skip_quotes(my_str, i, quote);
		if ((my_str[i] == '$' || my_str[i] == '~' || my_str[i] == '*')
			&& quote != '\'')
			my_str = single_expand(&i, my_str, vars);
		else
			i++;
	}
	return (my_str);
}

/* expands every variable found */
char	*just_expand_string(char *str, const char ***vars)
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
		if (my_str[i] == '$' || my_str[i] == '~')
			my_str = single_expand(&i, my_str, vars);
		else
			i++;
	}
	return (my_str);
}
