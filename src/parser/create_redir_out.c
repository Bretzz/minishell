/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_redir_out.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 16:05:30 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/28 18:16:08 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

// static int	silent_error_check(char *filename, char second_flag)
// {
// 	if (filename[0] == '\0')
// 	{
// 		return (1);
// 	}
// 	else if (ft_strlen(filename) > MAX_PATH)
// 	{
// 		return (1);
// 	}
// 	else if (access(filename, F_OK) != 0 && second_flag == R_OK)
// 	{
// 		return (1);
// 	}
// 	else if ((second_flag == R_OK && access(filename, second_flag) != 0)
// 		|| (second_flag == W_OK && (access(filename, F_OK) == 0
// 				&& access(filename, second_flag) != 0)))
// 	{
// 		return (1);
// 	}
// 	return (0);
// }

void	create_redir_out(t_token *tokens)
{
	int	fd;

	fd = 0;
	while (tokens)
	{
		if (tokens->type == TOKEN_RED_OUTPUT && tokens->next)
		{
			// if (silent_error_check(tokens->next->value, W_OK))
			// 	return ;
			fd = open(tokens->next->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			tokens = tokens->next;
		}
		else if (tokens->type == TOKEN_APPEND && tokens->next)
		{
			// if (silent_error_check(tokens->next->value, W_OK))
			// 	return ;
			fd = open(tokens->next->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
			tokens = tokens->next;
		}
		safeclose(fd);
		if (fd < 0)
			return ;
		tokens = tokens->next;
	}
}
