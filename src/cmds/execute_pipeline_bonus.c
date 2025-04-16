/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 11:29:08 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/16 11:45:28 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"
#include "cmds_bonus.h"

#define PIPE_FATHER 0	/* flag for exit pipeline procedure */
#define PIPE_CHILD 1	/* flag for exit pipeline procedure */

int	execute_pipeline_bonus(char *line, t_bush *bush, t_cmd *cmd, char ***vars);

static int	clean_exit(t_garb *garb, int index, t_bush *bush, char flag)
{
	int		exit_code;
	size_t	len;

	len = ft_cmdlen(garb->cmd);
	exit_code = EXIT_SUCCESS;
	if (flag == PIPE_CHILD)
	{
		child_closes(garb->cmd, garb, index, len);
		free(bush->line);
		free_bush(bush);
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
	t_bush *bush,
	int index)
{
	t_cmd	*cmd;
	char	***vars;

	cmd = garb->cmd;
	vars = garb->vars;
	clean_exit(garb, index, bush, PIPE_CHILD);
	single_execute(execfd, index, cmd, vars);
}

int	execute_pipeline_bonus(char *line, t_bush *bush, t_cmd *cmd, char ***vars)
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
				clean_exit(garb, i, bush, PIPE_FATHER));
		if (garb[i].pid == 0)
			im_trying_to_appreciate_norminette(execfd, garb, bush, i);
		father_closes(cmd, garb, i, len);
		i++;
	}
	return (clean_exit(garb, --i, bush, PIPE_FATHER));
}
