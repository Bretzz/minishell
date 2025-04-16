/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unbox_the_line_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 17:30:16 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/16 11:45:03 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"
#include "core_bonus.h"

int	unbox_the_line_bonus(char *line, char ***vars);

/* execute the command array passed.
If the 'exit' command is run as single command an exit(3) call
is performed with the exit code of the last foreground pipeline. */
static void	badass_executioner(char *line,
	t_cmd *cmd_arr,
	t_bush *bush,
	char ***vars)
{
	int		exit_code;
	size_t	len;

	len = ft_cmdlen(cmd_arr);
	exit_code = 0;
	if (*((unsigned char *)vars[0] + 6) == 1)
		runtime_initializer();
	if (len <= 1)
	{
		exit_code = execute_command_bonus(line, bush, cmd_arr, vars);
		if (exit_code < 0)
			clean_exit(cmd_arr, line, vars, -1 * (exit_code + 1));
		mtx_setdata(exit_code, vars[0], 1);
	}
	else
	{
		exit_code = execute_pipeline_bonus(line, bush, cmd_arr, vars);
		if (exit_code < 0)
			clean_exit(cmd_arr, line, vars, -1 * (exit_code + 1));
		mtx_setdata(exit_code, vars[0], 1);
	}
	if (*((unsigned char *)vars[0] + 6) == 1)
		idle_initializer();
}

/* execs the 'exec_line' passed, if exit(3) calls are made the whole
'line' ptr is free(3)d. */
static int	king_richard_the_third(char *line, char *exec_line,
	t_bush *bush, char ***vars)
{
	t_cmd	*cmd_arr;
	size_t	len;

	cmd_arr = NULL;
	if (line == NULL)
		return (1);
	cmd_arr = parse_tokens_bonus((char *)exec_line, (const char ***)vars);
	if (g_last_sig != 0)
	{
		mtx_setdata(128 + g_last_sig, vars[0], 1);
		return (free_cmd(cmd_arr), 1);
	}
	len = ft_cmdlen(cmd_arr);
	if (len == 0)
	{
		mtx_setdata(cmd_arr->parse_code, vars[0], 1);
		return (free_cmd(cmd_arr), 1);
	}
	badass_executioner(line, cmd_arr, bush, vars);
	free_cmd(cmd_arr);
	return (1);
}

static int	sir_bis(char *line, int skips, char ***vars)
{
	t_bush	*bush[2];

	bush[0] = line_to_bush((char *)line + skips);
	if (bush[0] == NULL)
	{
		write(STDERR_FILENO, "minishell: malloc failure\n", 26);
		return (0);
	}
	print_bush(bush[0]);	//remove
	bush[1] = bush[0];
	while (bush[1])
	{
		king_richard_the_third(line, bush[1]->sub_line, bush[0], vars);
		if (*((unsigned char *)vars[0] + 5) != 0)
			bush[1] = bush[1]->false;
		else
			bush[1] = bush[1]->true;
	}
	free_bush(bush[0]);
	return (1);
}

/* Well... */
static int	get_colon_count(char *line)
{
	size_t	i;
	int		colon_count;

	colon_count = 0;
	i = 0;
	while (line[i] != '\0')
	{
		if (line[i] == ';')
		{
			line[i] = '\0';
			colon_count++;
		}
		i++;
	}
	return (colon_count);
}

/* replaces ';' with '\0' and pass the skipped index
to king_richard_the_third(). */
int	unbox_the_line_bonus(char *line, char ***vars)
{
	int		null_skip;
	int		colon_count;
	size_t	i;

	colon_count = get_colon_count(line);
	null_skip = 0;
	i = 0;
	while (colon_count >= 0)
	{
		g_last_sig = 0;
		if (line[i] == '\0')
			null_skip = 0;
		if (line[i] != '\0' && !null_skip)
		{
			if (!sir_bis(line, i, vars))
				return (1);
			null_skip = 1;
			colon_count--;
		}
		i++;
	}
	return (1);
}
