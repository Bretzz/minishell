/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_line.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totommi <totommi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 11:32:32 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/08 02:01:47 by totommi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

char	*syntax_line(char *line);
char	*drop_comment(char *line);

static void	clean_exit(char *line, char *check_this, t_token *tokens)
{
	if (line != NULL)
		add_history(line);
	free(check_this);
	free_tokens(tokens);
}

static char	*drop_string(char *str, int start, size_t len)
{
	char	*new_str;
	char	*tar_ptr;
	int		i;

	if (str == NULL)
		return (NULL);
	//tar_len = ft_varlen(&str[tar_index]) + 1;
	tar_ptr = &str[start];
	new_str = (char *)ft_calloc(ft_strlen(str) - len + 1, sizeof(char));
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
			str += len;
	}
	return (new_str);
}

/* takes a string and returns a substr from 0 to the first '#' found
not inside quotes replaced with '\0' */
char	*drop_comment(char *line)
{
	size_t	i;
	char	quote;
	char	*my_line;
	char	*tmp;

	my_line = ft_strdup(line);
	if (my_line == NULL)
		return (NULL);
	quote = 0;
	i = 0;
	while (my_line[i] != '\0')
	{
		if (quote == 0
			&& (my_line[i] == '\'' || my_line[i] == '"'))
			quote = my_line[i];
		else if (my_line[i] == quote)
			quote = 0;
		if ((my_line[i] == '#') && quote == 0
			&& (i == 0 || ft_isspace(my_line[i - 1])))
		{
			tmp = drop_string(my_line, i, ft_strlen_nl(&my_line[i]));
			if (tmp == NULL)
			{
				write(STDERR_FILENO, "minishell: malloc failure\n", 26);
				return (NULL);
			}
			free(my_line);
			my_line = tmp;	
		}
		i++;
	}
	return (my_line);
}

static int	is_closed(char *line)
{
	char	quote;
	char	last;
	size_t	i;

	if (line == NULL)	//never happens
		return (0);
	last = 0;
	quote = 0;
	i = 0;
	while (line[i] != '\0')
	{
		if (!ft_isspace(line[i]))
			last = line[i];
		if (quote == 0
			&& (line[i] == '\'' || line[i] == '"'))
			quote = line[i];
		else if (line[i] == quote)
			quote = 0;
		i++;
	}
	if (last == '|' || quote != 0)
		return (0);
	return (1);
}

static void	cleanup(char *line)
{
	add_history(line);
	free(line);
	if (g_last_sig == SIGINT)
	{
		g_last_sig = 0;
		return ;
	}
	ft_printfd(STDERR_FILENO, "minishell: syntax error near unexpected end of file\n");
	return ;
}

static t_token_type get_last_type(t_token *tokens)
{
	if (tokens == NULL)
		return (TOKEN_WORD);
	while (tokens->next)
		tokens = tokens->next;
	return (tokens->type);
}

static int	is_white(char *str)
{
	size_t i;

	if (str == NULL)
		return (1);
	i  = 0;
	while (str[i] && ft_isspace(str[i]))
		i++;
	if (str[i] == '\0')
		return (1);
	return (0);
}

static char *real_line(const char *prompt)
{
	char	*real_line;
	
	real_line = ft_readline(prompt);
	while (real_line && is_white(real_line))
	{
		free(real_line);
		real_line = ft_readline(prompt);
	}
	return (real_line);
}

static char	*append_line(char *line, t_token_type last_type)
{
	char	*next_line;
	
	input_initializer();
	if (last_type == TOKEN_PIPE)
	{
		next_line = real_line("> ");
		if (next_line == NULL)
			return (cleanup(line), NULL);
		line = ft_strjoin_free_space(line, next_line);
		free(next_line);
	}
	else if (last_type == TOKEN_WORD)
	{
		next_line = ft_readline("> ");
		if (next_line == NULL)
			return (cleanup(line), NULL);
		line = ft_strjoin_free_nl(line, next_line);
		free(next_line);
	}
	idle_initializer();
	return (line);
}

/* checks weather a line is syntattically correct and expands it if necessary. */
char	*syntax_line(char *line)
{
	t_token	*tokens;
	char	*check_this;
	
	if (is_white(line))
		return (NULL);
	check_this = drop_comment(line);
	if (check_this == NULL)
		return (NULL);
	tokens = tokenizer(check_this);
	if (!syntax_tokens(tokens))
	{
		clean_exit(line, check_this, tokens);
		return (NULL);
	}
	free(check_this);
	while (!is_closed(line))
	{
		line = append_line(line, get_last_type(tokens));
		free_tokens(tokens);
		if (line == NULL)
			return (clean_exit(NULL, NULL, NULL), NULL);
		check_this = drop_comment(line);
		if (check_this == NULL)
		{
			return (clean_exit(line, check_this, NULL), NULL);
			//return (NULL);
		}
		tokens = tokenizer(check_this);
		if (!syntax_tokens(tokens))
		{
			return (clean_exit(line, check_this, tokens), NULL);
			//return (NULL);
		}
		free(check_this);
	}
	free_tokens(tokens);
	return (line);	
}
