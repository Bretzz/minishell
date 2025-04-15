/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safety_first.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 00:03:12 by march             #+#    #+#             */
/*   Updated: 2025/04/15 19:48:37 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "cmds.h"

char	*prompt_safe_line(const char *prompt, char sig_flag);
int		doc_exp_write(int fd, char exp_flag, char *line, const char ***vars);

/* returns a line read from the 'STDIN_FILENO'.
If sig_flag is set to 1 writes the prompt passed.
If a SIGQUIT or a SIGTSTP is intercepted tries again. */
char	*prompt_safe_line(const char *prompt, char sig_flag)
{
	char	*line;

	if (sig_flag == 1)
		write(STDOUT_FILENO, prompt, ft_strlen(prompt));
	line = get_safe_line(STDIN_FILENO);
	return (line);
}

/* takes the exp_flag, line to write and the fd as params.
If the flag is set to 0 the line is written as it is then freed, if not
the line is expanded. Then line is free'd.
RETURNS: 0 on malloc failure, 1 otherwise. */
int	doc_exp_write(int fd, char exp_flag, char *line, const char ***vars)
{
	char	*exp_line;

	if (exp_flag != 0)
	{
		exp_line = just_expand_string(line, (const char ***)vars);
		if (exp_line == NULL)
		{
			write(STDOUT_FILENO, "minishell malloc failure\n", 26);
			return (0);
		}
		else
		{
			write(fd, exp_line, ft_strlen(exp_line));
		}
		free(exp_line);
	}
	else
	{
		write(fd, line, ft_strlen(line));
	}
	return (1);
}
