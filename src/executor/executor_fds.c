/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_fds.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caide-so <caide-so@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 16:04:54 by caide-so          #+#    #+#             */
/*   Updated: 2025/05/14 16:05:58 by caide-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Saves the STDIN and STDOUT file descriptors.
// 1. Duplicates STDIN and STDOUT into save_stdin and save_stdout.
// 2. Exits on failure usint exit_perror().
// Returns 0 on success.
// Note: Used to restore I/O state after redirections.
int	save_fds(int *save_stdin, int *save_stdout)
{
	*save_stdin = dup(STDIN_FILENO);
	*save_stdout = dup(STDOUT_FILENO);
	if (*save_stdin < 0 || *save_stdout < 0)
		exit_perror("dup failed");
	return (0);
}

// Restores STDIN and STDOUT from saved descriptors.
// 1. Replaces STDIN and STDOUT using dup2.
// 2. Closes the saved descriptors.
// 3. Exits on error using exit_perror().
// Returns 0 on success.
int	restore_fds(int save_stdin, int save_stdout)
{
	if (dup2(save_stdin, STDIN_FILENO) < 0
		|| dup2(save_stdout, STDOUT_FILENO) < 0)
		exit_perror("dup2 restore failed");
	close(save_stdin);
	close(save_stdout);
	return (0);
}
