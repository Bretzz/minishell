/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 17:35:08 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/14 15:01:40 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "cmds.h"

int		open_doc(char flag);
void	close_docs(void);
int		read_doc(int doc_num);
char	*get_doc_path(int doc_num, char *buff, size_t size);

static char	*copy_path(char *cur_dir, char *itoa, char *buff, size_t size)
{
	if (buff == NULL)
	{
		size = ft_strlen(cur_dir) + ft_strlen(itoa) + 22;
		buff = malloc(size * sizeof(char));
	}
	ft_memset(buff, 0, size);
	if (ft_strlen(cur_dir) + ft_strlen(itoa) + 22 <= size)
	{
		ft_strlcpy(buff, cur_dir, size);
		ft_strlcat(buff, "/", size);
	}
	ft_strlcat(buff, "here_docs/here_doc_", size);
	ft_strlcat(buff, itoa, size);
	return (buff);
}

/* returns a string with the path:
	path/to/minishell/here_docs/here_doc_<doc_num> */
char	*get_doc_path(int doc_num, char *buff, size_t size)
{
	char	cur_dir[MAX_PATH + 1];
	char	*itoa;

	if (!getcwd(cur_dir, sizeof(cur_dir)) && errno == ENAMETOOLONG)
	{
		ft_perror("curdir", "File name too long", NULL, 0);
		return (NULL);
	}
	itoa = ft_itoa(doc_num);
	if (itoa == NULL)
	{
		write(STDERR_FILENO, "minishell: malloc failure\n", 26);
		return (NULL);
	}
	if (ft_strlen(cur_dir) + ft_strlen(itoa) + 19 > MAX_PATH)
	{
		ft_perror("curdir", "File name too long", NULL, 0);
		return (free(itoa), NULL);
	}
	copy_path(cur_dir, itoa, buff, size);
	return (free(itoa), buff);
}

/* unlink() call on all the previously opened heredocs */
void	close_docs(void)
{
	char			path[MAX_PATH + 1];
	unsigned int	i;
	unsigned int	doc_num;

	doc_num = open_doc(GETNUM);
	if (doc_num == 0)
		return ;
	i = 1;
	while (i <= doc_num)
	{
		if (!get_doc_path(i, path, sizeof(path)))
			return ;
		unlink(path);
		i++;
	}
	open_doc(RESET);
}

/* returns a O_RDONLY fd of the 'doc_num' heredoc. */
int	read_doc(int doc_num)
{
	char	path[MAX_PATH + 1];
	int		fd;

	if (doc_num == 0)
		return (-1);
	if (!get_doc_path(doc_num, path, sizeof(path)))
		return (-1);
	fd = open(path, O_RDONLY, 0644);
	if (fd < 0)
	{
		write(STDERR_FILENO, "minishell: heredoc failure\n", 27);
		return (-1);
	}
	return (fd);
}

/* if flag is set to CREATE, creates a new heredoc,
if flag is set to GETNUM, gets the number of the last here-document,
if flag is set to RESET, sets the heredoc number to 0,
otherwise returns the number of the last heredoc created,
0 if none was created. */
int	open_doc(char flag)
{
	static unsigned int	doc_num;
	int					fd;
	char				path[MAX_PATH + 1];

	if (flag == GETNUM)
		return (doc_num);
	else if (flag == RESET)
	{
		doc_num = 0;
		return (doc_num);
	}
	doc_num++;
	if (!get_doc_path(doc_num, path, sizeof(path)))
		return (-1);
	fd = open(path, O_WRONLY | O_CREAT, 0644);
	if (fd < 0)
	{
		write(STDERR_FILENO, "minishell: heredoc failure\n", 27);
		return (-1);
	}
	return (fd);
}
