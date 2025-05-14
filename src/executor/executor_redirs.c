/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_redirs.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caide-so <caide-so@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 16:03:20 by caide-so          #+#    #+#             */
/*   Updated: 2025/05/14 16:10:06 by caide-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	handle_redirections(t_command *cmd)
{
	apply_input_redir(cmd);
	apply_output_redir(cmd);
}

void	apply_intput_redir(t_command *cmd)
{
	int	fd;

	if (cmd->infile)
	{
		fd = open(cmd->infile, O_RDONLY);
		if (fd < 0)
			exit_perror("open infile");
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	if (cmd->heredoc)
	{
		fd = open(cmd->heredoc_path, O_RDONLY);
		if (fd < 0)
			exit_perror("open heredoc file");
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
}

void	apply_output_redir(t_command *cmd)
{
	int	fd;

	if (cmd->outfile)
	{
		fd = open(cmd->outfile, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (fd < 0)
			exit_perror("open outfile");
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	if (cmd->appendfile)
	{
		fd = open(cmd->appendfile, O_CREAT | O_WRONLY | O_APPEND, 0644);
		if (fd < 0)
			exit_perror("open appendfile");
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
}
