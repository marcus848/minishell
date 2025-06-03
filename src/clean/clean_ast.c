/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcudos <marcudos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 18:42:03 by marcudos          #+#    #+#             */
/*   Updated: 2025/05/29 04:30:32 by caide-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	free_heredocs(t_heredoc *hd);

void	ast_free(t_ast *root)
{
	if (!root)
		return ;
	ast_free(root->left);
	ast_free(root->right);
	if (root->type == NODE_COMMAND && root->cmd)
		command_free(root->cmd);
	free(root);
}

void	command_free(t_command *command)
{
	if (command->heredoc_fd >= 0)
		close(command->heredoc_fd);
	if (command->args)
		free_args(command);
	if (command->infile)
		free(command->infile);
	if (command->outfile)
		free(command->outfile);
	if (command->appendfile)
		free(command->appendfile);
	if (command->heredocs)
		free_heredocs(command->heredocs);
	free(command);
}

void	free_args(t_command *command)
{
	int	i;

	i = 0;
	while (i < command->arg_count)
		free(command->args[i++]);
	free(command->args);
}

void	free_heredocs(t_heredoc *hd)
{
	t_heredoc	*next;

	while (hd)
	{
		next = hd->next;
		free(hd->delimiter);
		free(hd);
		hd = next;
	}
}
