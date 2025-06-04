/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_status.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caide-so <caide-so@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 20:54:16 by caide-so          #+#    #+#             */
/*   Updated: 2025/05/23 03:18:55 by caide-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	set_last_status(t_shell *shell, int status)
{
	if (g_signal_status >= 0)
	{
		shell->last_status = g_signal_status;
		return ;
	}
	shell->last_status = status & 0xFF;
}

int	get_last_status(t_shell *shell)
{
	return (shell->last_status);
}
