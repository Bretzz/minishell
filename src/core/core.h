/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 20:03:03 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/15 17:33:16 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CORE_H
# define CORE_H

/* CLEAN EXIT */

void	clean_exit(t_cmd *cmd_arr, char *line, char ***vars, int code);
int		unbox_the_line(char *line, char ***vars);

#endif