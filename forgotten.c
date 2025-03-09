/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forgotten.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 19:39:59 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/09 22:32:31 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_putchar(int c);

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
