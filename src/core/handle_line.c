/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_line.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 20:06:24 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/14 20:09:14 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "core.h"

void	handle_line(char **line, char ***vars);

/* execute the command array passed.
If the 'exit' command is run as single command an exit(3) call
is performed with the exit code of the last foreground pipeline. */
static void	badass_executioner(char *line,
	t_cmd *cmd_arr,
	size_t len,
	char ***vars)
{
	int	exit_code;

	exit_code = 0;
	if (*((unsigned char *)vars[0] + 6) == 1)
		runtime_initializer();
	if (len <= 1)
	{
		exit_code = execute_command(line, cmd_arr, vars);
		if (exit_code < 0)
			clean_exit(cmd_arr, line, vars, -1 * (exit_code + 1));
		mtx_setdata(exit_code, vars[0], 1);
	}
	else
	{
		exit_code = execute_pipeline(line, cmd_arr, vars);
		if (exit_code < 0)
			clean_exit(cmd_arr, line, vars, -1 * (exit_code + 1));
		mtx_setdata(exit_code, vars[0], 1);
	}
	if (*((unsigned char *)vars[0] + 6) == 1)
		idle_initializer();
}

static int	king_richard_the_third(char *line, char ***vars)
{
	t_cmd	*cmd_arr;
	size_t	len;

	cmd_arr = NULL;
	if (line == NULL)
		return (1);
	cmd_arr = parse_tokens((char *)line, (const char ***)vars);
	if (g_last_sig != 0)
	{
		mtx_setdata(128 + g_last_sig, vars[0], 1);
		g_last_sig = 0;
		return (free_cmd(cmd_arr), 1);
	}
	len = ft_cmdlen(cmd_arr);
	if (len == 0)
	{
		mtx_setdata(cmd_arr->parse_code, vars[0], 1);
		return (free_cmd(cmd_arr), 1);
	}
	badass_executioner(line, cmd_arr, len, vars);
	free_cmd(cmd_arr);
	return (1);
}

/* adds the passed line to the history, then alloc(3)s
a new string without comments (#), free(3)s the originl
string and returns the new string. */
static char	*history_is_set(char *line)
{
	char	*strip_line;

	if (line == NULL)
		return (NULL);
	add_history(line);
	strip_line = drop_comment(line);
	free(line);
	return (strip_line);
}

void	handle_line(char **line, char ***vars)
{
	int	exit_code;

	if (*line == NULL)
	{
		if (*((unsigned char *)vars[0] + 6) == 1)
			write(STDOUT_FILENO, "exit\n", 5);
		clean_exit(NULL, NULL, vars, *((unsigned char *)vars[0] + 5));
	}
	exit_code = syntax_line(line, vars);
	if (exit_code == 2)
		bongou_stray_docs(*line, (const char ***)vars);
	else if (exit_code == 1)
		clean_exit(NULL, *line, vars, EXIT_FAILURE);
	*line = history_is_set(*line);
	if (*line != NULL && exit_code == 0 && !king_richard_the_third(*line, vars))
		clean_exit(NULL, *line, vars, EXIT_FAILURE);
}
