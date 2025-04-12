/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mtx.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 15:53:46 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/12 18:31:17 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MTX_H
# define MTX_H

# include "libft.h"
# include "varstr.h"

/* 
 * DOCUMENTATION 
 * A 'mtx' is an array of pointers to char (aka: char **).
 * The mtx_expand function takes the size of mtx and
 * reallocs with 16 more slots for pointers.
 * The current size is written in the first slot of the matrix:alignas
 * mtx[0] = (unsigned int)size | (unsigned int)0;
 * mtx[1] = (char *)var1;
 * mtx[2] = (char *)var2;
 * ...
 * ...
 * mtx[n] = (char *)varn;
 *
*/

/* MTX INIT */

char	**mtx_init(void);
char	**mtx_expand(char **mtx);

/* SETVAL */

char	**mtx_setval(char *name, char *value, char **mtx);
char	**mtx_setnum(char *name, int value, char **mtx);
char	**mtx_vstr_copy(char *varstr, char **mtx);

void	mtx_setdata(int value, char **mtx, int slot);

char	**mtx_addval(char *name, char *value, char **mtx);
char	**mtx_addnum(char *name, int value, char **mtx);
void	mtx_replace(char *name, char *value, char **mtx);

/* DELVAR */

void	mtx_delvar(int index, char **mtx);
void	mtx_safedel(int index, char **mtx);

/* GET INFO */

int		mtx_getindex(char *name, char **mtx);
char	*mtx_findval(char *name, char *buff, size_t size, char **mtx);

/* MOVE DATA */

void	mtx_move(int src, int dest, char **mtx);
void	mtx_pushup(char **mtx);

/* CLEANUP */

void	mtx_free(char **mtx);

#endif