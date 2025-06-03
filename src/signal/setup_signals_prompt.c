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

void	handle_sigint_prompt(int sig);

void	setup_signals_prompt(void)
{
	struct sigaction	sa_int;	

	sa_int = setup_sigint_prompt();
	sigaction(SIGINT, &sa_int, NULL);
	signal(SIGQUIT, SIG_IGN);
}

t_sig	setup_sigint_prompt(void)
{
	t_sig	sa_int;

	sa_int.sa_handler = handle_sigint_prompt;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	return (sa_int);
}

void	handle_sigint_prompt(int sig)
{
	(void) sig;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}
