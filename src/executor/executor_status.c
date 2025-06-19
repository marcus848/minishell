/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_status.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caide-so <caide-so@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 20:54:16 by caide-so          #+#    #+#             */
/*   Updated: 2025/06/06 11:08:25 by marcudos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Sets the shell's last_status based on global signal status.
// 1. If a signal was received (g_signal_status >= 0), updates status to
// special value (-2).
// 2. Otherwise, store exit status's lower 8 bits.
void	set_last_status(t_shell *shell, int status)
{
	if (g_signal_status >= 0)
	{
		update_sh_last_status(shell, -2);
		return ;
	}
	shell->last_status = status & 0xFF;
}

// Returns the last stored shell status.
int	get_last_status(t_shell *shell)
{
	return (shell->last_status);
}
