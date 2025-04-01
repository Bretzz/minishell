/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forgotten.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 19:39:59 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/01 22:24:41 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_putchar(int c);

/* sets the value of the var pointed by dest,
to the int passed by value
EXPECTED: "VARNAME=VALUE" string pointed by *dest.
NOTE: we need some allocated space after the '=', len indicates that
RETURNS: 1 on successfull execution, 0 on error.
NOTE2: as var_append(), also setnum() is safe to assign to the original matrix
since it preserve the original value even on malloc failures. */
char	**setnum(char **mtx, const char *target, int value)
{
	char	*str_val;
	char	new_var[23];	// max value = 9999999999(10 cifre), min value = -999999999(9 cifre)

	if (!target)
		return (mtx);
	str_val = (char *)ft_itoa(value);
	if (str_val == NULL)
	{
		ft_printfd(STDERR_FILENO, "minishell: malloc: Allocation failure (hopefully handled)\n");
		return (mtx);
	}
	ft_bzero(new_var, 23);
	ft_strlcpy(new_var, target, ft_strlen(target) + 1);
	ft_strlcat(new_var, "=", ft_strlen(new_var) + 2);
	ft_strlcat(new_var, str_val, ft_strlen(new_var) + ft_strlen(str_val) + 1);
	//ft_printf("var_append(ing): '%s'\n", new_var);
	mtx = var_append(mtx, new_var);
	free(str_val);
	return (mtx);
}

/* RETURNS: 1 on good execution, 0 on error */
/* int	change_value(char **dest, char *src)
{
	char	*new_dest;
	int		src_len;
	int		eq;

	if (!dest || !*dest)
		return (0);
	//free(*dest);
	eq = ft_strichr(*dest, '=');
	src_len = ft_strlen(src);
	new_dest = ft_realloc(*dest, eq * sizeof(char), (eq + src_len) * sizeof(char));
	if (new_dest == NULL)
		return (NULL);
	ft_memcpy(*dest)
} */

/* we're expecting to find an mtx_var with same key as var at the index 'index',
so we free that string and replace it with var.
RETURNS: a pointer to the matrix, NULL on malloc failure or NULL mtx. */
static char	**replace_var(int index, char **mtx, char *var)
{
	if (mtx == NULL)
		return (NULL);
	free(mtx[index]);
	mtx[index] = ft_strdup(var);
	if (mtx[index] == NULL)
	{
		ft_printfd(2, "malloc failre\n");
		//ft_freentf("2", mtx);
		return (NULL);
	}
//	ft_print_charr((const char **)mtx);
	return (mtx);
}

/* adds a new var_string allocated with malloc(3) to the passed matrix.
If the var_string is NULL the var_append just returns the ptr to the matrix. 
var_string syntax: varname=varvalue.
NOTE: the var_string must be a NULL-terminated string.
RETURNS: a pointer to the head of the matrix, NULL if a malloc fails.
NOTE: is safe to assign the originale matrix to the return of var_append,
since if it fails somehow just returns the old matrix.
NOTE2: a valid var is expected, var_is_valid() function check. */
char	**var_append(char **mtx, char *var)
{
	char	**new_matrix;
	size_t	len;
	int		index;

//	ft_print_charr((const char **)mtx);
	if (var == NULL)
		return (mtx);
	len = ft_mtxlen((const void **)mtx);
	index = is_there((const char **)mtx, var);
	if (index >= 0)
		return (replace_var(index, mtx, var));
	new_matrix = ft_realloc(mtx, len * sizeof(char *), (len + 2) * sizeof(char *));
	if (new_matrix != NULL)
	{
		new_matrix[len] = ft_strdup(var);
		if (new_matrix[len] != NULL)
		{
			free(mtx);
			new_matrix[len + 1] = NULL;
			//ft_print_charr((const char **)new_matrix);
			return (new_matrix);
		}
		free(new_matrix);
	}
	//not freeing so the arr is still usable
	ft_printfd(STDERR_FILENO, "minishell: malloc error that maybe is handled\n");
	return (mtx);
}

/* initialize the terminal.
RETURNS: 1 if all good, 0 on error. */
int	term_init(void)
{	
	// Initialize the terminal
    char *termtype = getenv("TERM");
    if (termtype == NULL)
	{
        ft_printfd(STDERR_FILENO, "No TERM environment variable set\n");
        return (0);
    }

    // Set up terminfo database and check if it's available
    int result = tgetent(NULL, termtype);
    if (result == -1)
	{
        ft_printfd(STDERR_FILENO, "Could not access the terminfo database\n");
        return (0);
    }
	else if (result == 0)
	{
        ft_printfd(STDERR_FILENO, "Terminal type '%s' is not defined in terminfo\n", termtype);
        return (0);
    }
	return (1);
}

/* clears the terminal.
RETURNS: 1 if all good, 0 on error. */
int	term_clear(void)
{
	// Get the 'clear' string capability from the terminal's terminfo
    char *clear_str = tgetstr("cl", NULL);
    if (clear_str == NULL)
	{
        ft_printfd(STDERR_FILENO, "Clear capability is not available\n");
        return (0);
    }

    // Use tputs to send the escape sequence to clear the screen
    tputs(clear_str, 1, ft_putchar);
	return (1);
}
