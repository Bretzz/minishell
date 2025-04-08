/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   term_set.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 14:30:26 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/08 20:15:56 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_readline.h"

void	set_terminal_mode(struct termios *old_tio);
void	restore_terminal_mode(struct termios *old_tio);
int		is_input_ready(int fd);

// Function to set the terminal to non-canonical mode (for non-blocking input)
void	set_terminal_mode(struct termios *old_tio)
{
	struct termios new_tio;
	
	// Get the current terminal attributes
	tcgetattr(STDIN_FILENO, old_tio);
	
	// Copy the old attributes to new_tio and modify for non-canonical mode
	new_tio = *old_tio;
	
	// Disable canonical mode (no line buffering) and disable echo
	new_tio.c_lflag &= ~(ICANON | ECHO);	// Disable canonical mode and echo
	new_tio.c_cc[VMIN] = 1;					// Minimum number of characters to read (1 character)
	new_tio.c_cc[VTIME] = 0;				// Timeout (0 means no timeout)
	
	// Apply the new terminal attributes immediately
	tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
}

// Function to restore the terminal to its original mode
void	restore_terminal_mode(struct termios *old_tio)
{
	tcsetattr(STDIN_FILENO, TCSANOW, old_tio);
}

// Function to check if input is available on STDIN without waiting for Enter
int	is_input_ready(int fd)
{
	int	bytes_available;

	ioctl(fd, FIONREAD, &bytes_available);
	if (bytes_available == 0)
		return (0);
	return (1);
}
