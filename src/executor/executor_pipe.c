/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pipe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caide-so <caide-so@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 03:10:39 by caide-so          #+#    #+#             */
/*   Updated: 2025/05/26 03:11:15 by caide-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Handles execution logic for the left side of a pipe.
// 1. Sets execution signal handlers.
// 2. Redirects STDOUT to pipe write end.
// 3. Closes both pipe ends.
// 4. Executes the AST node.
// 5. Cleans up and exits with last status.
// Note: Used in the left child process of a fork during piping;
void	handle_left_child(int *fd, t_shell *shell, t_ast *node)
{
	setup_signals_exec();
	dup2(fd[1], STDOUT_FILENO);
	close(fd[0]);
	close(fd[1]);
	executor(shell->tokens, shell, node);
	clean_all(shell->tokens, shell->ast, &shell->env);
	exit(get_last_status(shell));
}

// Handles execution logic for the right side of a pipe.
// 1. Sets execution signal handlers.
// 2. Redirects STDIN to pipe read end.
// 3. Closes both pipe ends.
// 4. Executes the AST node.
// 5. Cleans up and exits with last status.
// Note: Used in the right child process of a fork during piping;
void	handle_right_child(int *fd, t_shell *shell, t_ast *node)
{
	setup_signals_exec();
	dup2(fd[0], STDIN_FILENO);
	close(fd[1]);
	close(fd[0]);
	executor(shell->tokens, shell, node);
	clean_all(shell->tokens, shell->ast, &shell->env);
	exit(get_last_status(shell));
}
