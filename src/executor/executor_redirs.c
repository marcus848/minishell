/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_redirs.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caide-so <caide-so@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 16:03:20 by caide-so          #+#    #+#             */
/*   Updated: 2025/06/10 00:52:05 by caide-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	finish_redirections(int ret, int in_fd, int out_fd);

// Applies all I/O redirections for a command.
// 1. Iterates through redirection list in cmd.
// 2. Handles input redirection (R_IN) with handle_in_redir().
// 3. Handles heredoc (R_HEREDOC) with handle_heredoc.
// 4. Handles output/append redirection (R_OUT, R_APPEND) with handle_out_redir.
// 5. Stops on error and proceeds to finish_redirections.
// Note: All open FDs are managed and closed appropriately.
int	apply_redirections(t_command *cmd, t_shell *sh)
{
	t_redir	*redir;
	int		in_fd;
	int		out_fd;
	int		ret;

	redir = cmd->redirs;
	in_fd = -1;
	out_fd = -1;
	ret = 0;
	while (redir && ret >= 0)
	{
		if (redir->type == R_IN)
			ret = handle_in_redir(redir, &in_fd, sh);
		else if (redir->type == R_HEREDOC)
			ret = handle_heredoc(redir, &in_fd);
		else if (redir->type == R_OUT || redir->type == R_APPEND)
			ret = handle_out_redir(redir, &out_fd, sh);
		if (ret >= 0)
			redir = redir->next;
	}
	return (finish_redirections(ret, in_fd, out_fd));
}

// Finalizes redirections by duplicating or closing FDs.
// 1. If error (ret < 0), closes any open FDs and returns -1.
// 2. If input FD is valid, redirects STDIN using dup2_close().
// 3. If output FD is valid, redirects STDOUT using dup2_close().
// Returns 0 on success.
int	finish_redirections(int ret, int in_fd, int out_fd)
{
	if (ret < 0)
	{
		if (in_fd != -1)
			close(in_fd);
		if (out_fd != -1)
			close(out_fd);
		return (-1);
	}
	if (in_fd != -1)
		dup2_close(in_fd, STDIN_FILENO);
	if (out_fd != -1)
		dup2_close(out_fd, STDOUT_FILENO);
	return (0);
}
