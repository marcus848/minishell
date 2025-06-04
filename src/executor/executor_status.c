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
	shell->last_status = status & 0xFF;
}

int	get_last_status(t_shell *shell)
{
	return (shell->last_status);
}

int	exit_status_from_wait(int status)
{
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (WEXITSTATUS(status));
}
