/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   varstr.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 18:52:44 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/01 13:27:08 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VARSTR_H
# define VARSTR_H

# include "libft.h"

/* 
 *	VAR-STRING: "name=value", where:
 *		name belongs to the[a-z, A-Z, _] charset,
 *		value can be anything.
*/

# define MAX_NAME 1024

char	*vstr_getname(char *varstr, char *buff, size_t size);
char	*vstr_getvalue(char *varstr, char *buff, size_t size);

int		vstr_name_is_valid(char *varstr);

#endif