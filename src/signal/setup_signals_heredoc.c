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

void    setup_signals_heredoc(void)
{
        t_sig   sa_int;

        sa_int.sa_handler = handle_sigint_heredoc;
        sigemptyset(&sa_int.sa_mask);
        sa_int.sa_flags = 0;
        sigaction(SIGINT, &sa_int, NULL);
        signal(SIGQUIT, SIG_IGN);
}

void	handle_sigeof_heredoc(char *delim)
{
	write(1, "minishell: warning: here-document delimited by end-of-file", 58);
	write(1, " (wanted '", 10);
	write(1, delim, ft_strlen(delim));
	write(1, "')\n", 3);
}

void	handle_sigint_heredoc(int sig)
{
	(void) sig;
	g_signal_status = 130;
}
