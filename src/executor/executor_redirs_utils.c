/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_redirs_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caide-so <caide-so@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 00:51:34 by caide-so          #+#    #+#             */
/*   Updated: 2025/06/10 00:51:52 by caide-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	open_file_with_error(char *filename, int flags, int mode, t_shell *sh);

// Handles input redirections from a file.
// 1. Opens a file in read-only mode.
// 2. Closes previously opened input FD, if any.
// 3. Stores new FD in *in_fd.
// Note: Returns -1 on failure, 0 on success.
int	handle_in_redir(t_redir *redir, int *in_fd, t_shell *sh)
{
	int	fd;

	if (redir->filename == NULL)
		return (-1);
	fd = open_file_with_error(redir->filename, O_RDONLY, 0, sh);
	if (fd < 0)
		return (-1);
	if (*in_fd != -1)
		close(*in_fd);
	*in_fd = fd;
	return (0);
}

// Opens a file and prints error to stderr on failure.
// 1. Tries to open file with given flags and mode.
// 2. On failure, prints error and sets last_status to 1.
// 3. Returns file descriptor or -1.
int	open_file_with_error(char *filename, int flags, int mode, t_shell *sh)
{
	int	fd;

	fd = open(filename, flags, mode);
	if (fd < 0)
	{
		ft_putstr_fd("-minishell: ", STDERR_FILENO);
		ft_putstr_fd(filename, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putstr_fd(strerror(errno), STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
		sh->last_status = 1;
	}
	return (fd);
}

// Sets input file descriptor from heredoc.
// 1. Closes previously opened input FD, if any.
// 2. Uses heredoc_fd as new input.
// Note: Assumes heredoc_fd is already valid.
int	handle_heredoc(t_redir *redir, int *in_fd)
{
	if (*in_fd != -1)
		close(*in_fd);
	*in_fd = redir->heredoc_fd;
	return (0);
}

// Handles output or append redirection to a file.
// 1. Computes flags based on redir type.
// 2. Opens file with appropriate flags and mode.
// 3. Closes previously opened output FD, if any.
// 4. Stores new FD in *out_fd.
// Note: Returns -1 on failure, 0 on success.
int	handle_out_redir(t_redir *redir, int *out_fd, t_shell *sh)
{
	int	flags;
	int	fd;

	if (redir->filename == NULL)
		return (-1);
	flags = O_CREAT | O_WRONLY;
	if (redir->type == R_OUT)
		flags |= O_TRUNC;
	else
		flags |= O_APPEND;
	fd = open_file_with_error(redir->filename, flags, 0644, sh);
	if (fd < 0)
		return (-1);
	if (*out_fd != -1)
		close(*out_fd);
	*out_fd = fd;
	return (0);
}

// Replaces std_fd with fd and closes fd.
// 1. Duplicates fd to std_fd using dup2.
// 2. Closes original fd.
void	dup2_close(int fd, int std_fd)
{
	dup2(fd, std_fd);
	close(fd);
}
