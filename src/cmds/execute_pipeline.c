/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 11:58:38 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/11 14:54:13 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "cmds.h"

#define PIPE_FATHER 0	/* flag for exit pipeline procedure */
#define PIPE_CHILD 1	/* flag for exit pipeline procedure */

int	execute_pipeline(char *line, t_cmd *cmd_arr, char ***vars);

static int	clean_exit(t_garb *garb, int index, size_t len, char flag)
{
	int	exit_code;

	exit_code = EXIT_SUCCESS;
	if (flag == PIPE_CHILD)
	{
		child_closes(garb->cmd, garb, index, len);
		free(garb->line);
	}
	else if (flag == PIPE_FATHER)
	{
		father_closes(garb->cmd, garb, index, len);
		exit_code = mass_wait(garb, index + 1);
	}
	free(garb);
	return (exit_code);
}

static t_garb	garb_init(char *line, t_cmd *cmd, char ***vars)
{
	t_garb	garb;

	ft_memset(&garb, 0, sizeof(t_garb));
	garb.line = line;
	garb.cmd = cmd;
	garb.vars = vars;
	return (garb);
}

static t_garb	*garb_alloc(t_cmd *cmd, size_t *len)
{
	t_garb	*garb;

	*len = ft_cmdlen(cmd);
	if (*len == 0)
		return (NULL);
	garb = (t_garb *)malloc(*len * sizeof(t_garb));
	if (garb == NULL)
	{
		write(STDERR_FILENO, "minishell: malloc failure\n", 26);
		return (NULL);
	}
	return (garb);
}

static void	im_trying_to_appreciate_norminette(
	int *execfd,
	t_garb *garb,
	int index,
	size_t len)
{
	t_cmd	*cmd;
	char	***vars;

	cmd = garb->cmd;
	vars = garb->vars;
	clean_exit(garb, index, len, PIPE_CHILD);
	single_execute(execfd, index, cmd, vars);
}

int	execute_pipeline(char *line, t_cmd *cmd, char ***vars)
{
	t_garb			*garb;
	size_t			len;
	unsigned int	i;
	int				execfd[2];

	garb = garb_alloc(cmd, &len);
	if (garb == NULL)
		return (EXIT_FAILURE);
	i = 0;
	while (i < len)
	{
		garb[i] = garb_init(line, cmd, vars);
		execfd[0] = pipeline_redir_input(cmd, garb, i, len);
		execfd[1] = pipeline_redir_output(cmd, garb, i, len);
		garb[i].pid = fork();
		if (garb[i].pid < 0)
			return (ft_printfd(STDERR_FILENO,
					"minishell: %s: fork failure\n", cmd[i].words[0]),
				clean_exit(garb, i, len, PIPE_FATHER));
		if (garb[i].pid == 0)
			im_trying_to_appreciate_norminette(execfd, garb, i, len);
		father_closes(cmd, garb, i, len);
		i++;
	}
	return (clean_exit(garb, --i, len, PIPE_FATHER));
}
