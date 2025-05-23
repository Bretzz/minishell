/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_string_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 12:19:02 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/30 20:44:23 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

char	*single_expand_bonus(size_t *i, size_t *cut_here,
			char *str, const char ***vars);

static char	*set_exp_val(size_t *i, char *str, const char ***vars)
{
	if (str[*i] == '*')
		return (get_wild_value(i, str));
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

static char	*string_assemble(char *str, char *exp_val, int i)
{
	size_t	var_len;
	char	*new_str;

	if (str[i] == '*')
		var_len = ft_strlen_space(&str[i]);
	else
		var_len = ft_varlen(&str[i]) + 1;
	new_str = (char *)ft_calloc(ft_strlen(str) - var_len
			+ ft_strlen(exp_val) + 1, sizeof(char));
	if (new_str == NULL)
		return (NULL);
	ft_strlcpy(new_str, str, i + 1);
	ft_strlcat(new_str, exp_val, ft_strlen(new_str) + ft_strlen(exp_val) + 1);
	ft_strlcat(new_str, &str[i + var_len],
		ft_strlen(new_str) + ft_strlen(&str[i + var_len]) + 1);
	return (new_str);
}

/* takes a string (to be free'd) and the index of the var found.
Allocates with malloc(3) a new string that's a copy of the imput string
with the var_string '$var' replaced with it's value found in shv, exp or env,
then frees the input string.
RETURNS: A pointer to the new string, NULL on malloc failures. */
char	*single_expand_bonus(size_t *i, size_t *cut_here,
	char *str, const char ***vars)
{
	char	*exp_val;
	char	*new_str;

	exp_val = set_exp_val(i, str, vars);
	if (exp_val == NULL)
	{
		if (str[*i] == '$' || str[*i] == '~')
			return (free(str), drop_string(str, *i, ft_varlen(&str[*i]) + 1));
		return (str);
	}
	new_str = string_assemble(str, exp_val, *i);
	if (new_str == NULL)
		return (free(exp_val), NULL);
	if (cut_here)
		(*cut_here) = ft_strichr(exp_val, ' ');
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
char	skip_quotes(char *str, size_t i, char quote)
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
			my_str = single_expand_bonus(&i, NULL, my_str, vars);
		else
			i++;
	}
	return (my_str);
}
