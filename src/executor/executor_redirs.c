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

void	handle_redirections(t_command *cmd, t_shell *shell)
{
	apply_input_redir(cmd, shell);
	apply_output_redir(cmd);
}

int	apply_input_redir(t_command *cmd, t_shell *shell)
{
	int		fd;
	char	*last_filename;

	if (cmd->infiles)
	{
		if (validate_infiles(cmd->infiles, shell) < 0)
			return (-1);
		last_filename = get_last_infile(cmd->infiles);
		fd = open(last_filename, O_RDONLY);
		if (fd < 0)
			exit_perror("open infile");
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	if (cmd->heredocs)
	{
		fd = cmd->heredoc_fd;
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	return (0);
}

void	apply_output_redir(t_command *cmd)
{
	int		fd;
	char	*last_filename;

	if (cmd->outfiles)
		validate_out_append(cmd->outfiles, O_CREAT | O_WRONLY | O_TRUNC);
	if (cmd->appendfiles)
		validate_out_append(cmd->outfiles, O_CREAT | O_WRONLY | O_APPEND);
	if (cmd->appendfiles)
		last_filename = get_last_out_append(cmd->appendfiles);
	else if (cmd->outfiles)
		last_filename = get_last_out_append(cmd->outfiles);
	else
		return ;
	if (cmd->appendfiles)
		fd = open(last_filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
	else
		fd = open(last_filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
		exit_perror("open redirect");
	dup2(fd, STDOUT_FILENO);
	close(fd);
}
