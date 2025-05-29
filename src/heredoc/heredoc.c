/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caide-so <caide-so@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 00:01:06 by caide-so          #+#    #+#             */
/*   Updated: 2025/05/29 00:14:48 by caide-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	prepare_heredocs(t_ast *node)
{
	if (!node)
		return ;
	if (node->type == NODE_COMMAND && node->cmd->heredoc)
		node->cmd->heredoc_fd = process_heredoc(node->cmd->delimiter);
	prepare_heredocs(node->left);
	prepare_heredocs(node->right);
}

int	process_heredoc(char *delimiter)
{
	int	fds[2];
	char	*line;

	if (pipe(fds) < 0)
		exit_perror("pipe");
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, delimiter) == 0)
			break ;
		write(fds[1], line, ft_strlen(line));
		write(fds[1], "\n", 1);
		free(line);
	}
	free(line);
	close(fds[1]);
	return (fds[0]);
}
