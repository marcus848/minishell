/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_signals_prompt.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcudos <marcudos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 17:07:26 by marcudos          #+#    #+#             */
/*   Updated: 2025/06/03 20:18:24 by marcudos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Configures signal handlers for interactive prompt.
// 1. Sets up custom SIGINT handler with line editing support.
// 2. Ignores SIGQUIT signals.
void	setup_signals_prompt(void)
{
	struct sigaction	sa_int;	

	sa_int = setup_sigint_prompt();
	sigaction(SIGINT, &sa_int, NULL);
	signal(SIGQUIT, SIG_IGN);
}

// Prepares SIGQUIT handler configuration for internactive prompt.
// 1. Sets custom handler function.
// 2. Initializes empty signal mask.
// 3. Enables system call restart flag.
t_sig	setup_sigint_prompt(void)
{
	t_sig	sa_int;

	sa_int.sa_handler = handle_sigint_prompt;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	return (sa_int);
}

// Handles SIGINT during interactive prompt.
// 1. Sets global signal status to 130 (interrupt).
// 2. Prints newline and resets readline display.
// 3. Preserves prompt functionality.
void	handle_sigint_prompt(int sig)
{
	(void) sig;
	g_signal_status = 130;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

// Updates shell status values.
// 1. Stores previous signal status in shell struct.
// 2. Updates global signal status with new value.
void	update_sh_last_status(t_shell *sh, int new_value)
{
	sh->last_status = g_signal_status;
	g_signal_status = new_value;
}
