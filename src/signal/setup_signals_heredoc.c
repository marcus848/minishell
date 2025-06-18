/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_signals_heredoc.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcudos <marcudos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 10:13:21 by marcudos          #+#    #+#             */
/*   Updated: 2025/06/06 11:15:03 by marcudos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Configures signal handlers for heredoc processing.
// 1. Disables terminal echo control characters.
// 2. Sets custom SIGINT handler.
// 3. Ignores SIGQUIT signals.
void	setup_signals_heredoc(void)
{
	t_sig	sa_int;

	disable_echoctl();
	sa_int.sa_handler = handle_sigint_heredoc;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sigaction(SIGINT, &sa_int, NULL);
	signal(SIGQUIT, SIG_IGN);
}

// Handles EOF condition during heredoc input.
// 1. Prints warning message about missing delimiter.
// 2. Displays the expected delimiter.
void	handle_sigeof_heredoc(char *delim)
{
	write(1, "\n", 1);
	write(1, "minishell: warning: here-document delimited by end-of-file", 58);
	write(1, " (wanted '", 10);
	write(1, delim, ft_strlen(delim));
	write(1, "')\n", 3);
}

// Handles SIGINT during heredoc processing.
// 1. Sets global signal status to 130 (interrupt).
// 2. Prints newline for clean output.
void	handle_sigint_heredoc(int sig)
{
	(void)sig;
	g_signal_status = 130;
	write(1, "\n", 1);
}

// Disables terminal echo control character display.
// 1. Gets current terminal attributes.
// 2. Clears ECHOCTL flag.
// 3. Applies new terminal settings.
void	disable_echoctl(void)
{
	struct termios	term;

	tcgetattr(0, &term);
	term.c_lflag &= ~ECHOCTL;
	tcsetattr(0, TCSANOW, &term);
}

// Re-enables terminal echo control character display.
// 1. Gets current terminal attributes.
// 2. Sets ECHOCTL flag.
// 3. Applies new terminal settings.
void	enable_echoctl(void)
{
	struct termios	term;

	tcgetattr(0, &term);
	term.c_lflag |= ECHOCTL;
	tcsetattr(0, TCSANOW, &term);
}
