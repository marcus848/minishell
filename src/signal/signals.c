/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcudos <marcudos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 19:40:35 by marcudos          #+#    #+#             */
/*   Updated: 2025/06/03 20:13:53 by marcudos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
static void	sig_int_sh(int sig);
int	g_in_prompt;

void	set_std_sig()
{
	t_sig	sa_sigint;

	ft_memset(&sa_sigint, 0, sizeof(sa_sigint));
	sa_sigint.sa_handler = sig_int_sh;
	sa_sigint.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa_sigint, NULL);
	signal(SIGQUIT, SIG_IGN);
}

static void	sig_int_sh(int sig)
{
	if (sig == SIGINT && g_in_prompt)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		// write(1,"\nteste > ", 8);
	}
}
