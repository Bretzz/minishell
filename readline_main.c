/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_main.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totommi <totommi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:00:45 by topiana-          #+#    #+#             */
/*   Updated: 2025/04/08 23:50:27 by totommi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//cc -Wall -Werror -Wextra remain.c src/utils/readline/* src/utils/cleanup.c libft/libft.a -Ilibft -Isrc/vars/mtxdb -Isrc/utils/readline -I.

#include "minishell.h"
#include "ft_readline.h"
#include <signal.h>

int	g_last_sig = 0;

static void ft_readline_handler(int signal)
{
    if (signal == SIGINT)
    {
        // Stampa un newline (così se sei in un comando come "cat", va a capo)
        write(STDOUT_FILENO, "\n", 1);    //heheh
        // Cancella la linea corrente e prepara readline per una nuova linea
    }
    else if (signal == SIGQUIT)
    {
	//	write(STDOUT_FILENO, "\n", 1);    //heheh
	//	rl_replace_line("Pterodattilo!\n", 14);
		write(STDERR_FILENO, "\b\b  \b\b", 6);    //heheh
        //ft_printf("Pterodattilo!\n");
		// rl_replace_line("", 0);   //MacOS issues
        // //rl_on_new_line();
        // rl_redisplay();
    }
	g_last_sig = signal;
}

void	ft_readline_initializer(void)
{
	struct sigaction	come_and_get_me;

	bzero(&come_and_get_me, sizeof(come_and_get_me));
	come_and_get_me.sa_handler = ft_readline_handler;
	come_and_get_me.sa_flags = 0;
	sigaction(SIGINT, &come_and_get_me, NULL);
	sigaction(SIGQUIT, &come_and_get_me, NULL);
	//rl_catch_signals = 0;
}

int main(void)
{
	char	*line;
	
	ft_readline_initializer();
	while ((line = ft_readline("i_ram_your_ass> ")))
	{
		ft_printf("u just typed '%s'\n", line);
		if (line != NULL)
			ft_add_history(line);
		//write(STDOUT_FILENO, "u sure?\n", 8);
		free(line);
	}
	ft_free_history();
	return (0);
}

// void	set_terminal_mode(struct termios *old_tio)
// {
// 	struct termios new_tio;
	
// 	// Get the current terminal attributes
// 	tcgetattr(STDIN_FILENO, old_tio);
	
// 	// Copy the old attributes to new_tio and modify for non-canonical mode
// 	new_tio = *old_tio;
	
// 	// Disable canonical mode (no line buffering) and disable echo
// 	new_tio.c_lflag &= ~(ICANON | ECHO);	// Disable canonical mode and echo
// 	new_tio.c_cc[VMIN] = 1;					// Minimum number of characters to read (1 character)
// 	new_tio.c_cc[VTIME] = 0;				// Timeout (0 means no timeout)
	
// 	// Apply the new terminal attributes immediately
// 	tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
// }

// // Function to restore the terminal to its original mode
// void	restore_terminal_mode(struct termios *old_tio)
// {
// 	tcsetattr(STDIN_FILENO, TCSANOW, old_tio);
// }

// int main(void)
// {
// 	char	buff[10];
// 	ssize_t bytes_read;
// 	struct termios	old_tio;
// 	int	bybye;
// 	int	bytes_available;

// 	bybye = 0;
// 	// Set terminal to non-canonical mode for non-blocking input
// 	set_terminal_mode(&old_tio);
// 	while (bybye < 10 || ioctl(STDIN_FILENO, FIONREAD, &bytes_available))
// 	{
// 		bytes_read = read(STDIN_FILENO, buff, 1);
// 		if (bytes_read == 0)
// 			break ;
// 		printf("just read %zd bytes: %s\n", bytes_read, buff);
// 	}
// 	restore_terminal_mode(&old_tio);
// 	return (0);
// }
