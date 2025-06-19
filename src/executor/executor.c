/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caide-so <caide-so@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 21:46:29 by caide-so          #+#    #+#             */
/*   Updated: 2025/06/15 14:04:59 by caide-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	exec_command(t_token_list *tokens, t_shell *shell, t_ast *node);
void	exec_pipe(t_shell *shell, t_ast *node);
void	exec_subshell(t_token_list *tokens, t_shell *shell, t_ast *node);

// Executes AST nodes based on their type.
// 1. For NODE_COMMAND: expands arguments and executes the command.
// 2. For NODE_PIPE: executes the pipe sequence.
// 3. For NODE_AND: executes left node; if successful, executes right node.
// 4. For NODE_OR: executes left node; if failed and no signal interrupt,
// executes right node.
// 5. For NODE_SUBSHELL: executes the subshell node.
// Note: Recursively traverses AST to execute commands and control flow.
void	executor(t_token_list *tokens, t_shell *sh, t_ast *node)
{
	if (!node)
		return ;
	if (node->type == NODE_COMMAND)
	{
		expander(&node->cmd->args, sh->env, &node->cmd->arg_count, sh);
		exec_command(tokens, sh, node);
	}
	else if (node->type == NODE_PIPE)
		exec_pipe(sh, node);
	else if (node->type == NODE_AND)
	{
		executor(tokens, sh, node->left);
		if (get_last_status(sh) == 0)
			executor(tokens, sh, node->right);
	}
	else if (node->type == NODE_OR)
	{
		executor(tokens, sh, node->left);
		if (get_last_status(sh) != 0 && g_signal_status == -1)
			executor(tokens, sh, node->right);
	}
	else if (node->type == NODE_SUBSHELL)
		exec_subshell(tokens, sh, node->left);
	return ;
}

// Executes a simple command node.
// 1. Expands redirections.
// 2. Attempts to handle exit builtin.
// 3. Attempts to handle other builtins.
// 4. Executes external commands if no builtin mathched.
void	exec_command(t_token_list *tokens, t_shell *shell, t_ast *node)
{
	char	**args;

	args = node->cmd->args;
	expand_redirs(node->cmd->redirs, shell);
	if (try_exit(tokens, args, shell))
		return ;
	if (try_other_builtin(args, node->cmd, shell))
		return ;
	run_external_cmd(args, node->cmd, shell);
}

// Executes a pipe node connecting two commands.
// 1. Creates a pipe.
// 2. Forks two child processes for left and right nodes.
// 3. Handles each child's execution and pipes.
// 4. Closes pipe fds in parent.
// 5. Waits for both children and updates last status.
void	exec_pipe(t_shell *shell, t_ast *node)
{
	int		fd[2];
	pid_t	left_pid;
	pid_t	right_pid;

	if (pipe(fd) == -1)
		exit_perror("pipe");
	left_pid = fork();
	if (left_pid == 0)
		handle_left_child(fd, shell, node->left);
	right_pid = fork();
	if (right_pid == 0)
		handle_right_child(fd, shell, node->right);
	close(fd[0]);
	close(fd[1]);
	waitpid(left_pid, &shell->last_status, 0);
	waitpid(right_pid, &shell->last_status, 0);
	set_last_status(shell, WEXITSTATUS(shell->last_status));
}

// Executes a subshell node.
// Forks a child process to execute the subtree.
// Child sets up signals, execute subtree, cleans up, and exits.
// Parent waits for child and updates last_status.
// Note: Subshell executes in separate process context.
void	exec_subshell(t_token_list *tokens, t_shell *shell, t_ast *node)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
		exit_perror("fork");
	if (pid == 0)
	{
		setup_signals_exec();
		executor(tokens, shell, node);
		clean_all(shell->tokens, shell->ast, &shell->env);
		exit(shell->last_status);
	}
	if (waitpid(pid, &status, 0) < 0)
		exit_perror("waitpid");
	shell->last_status = WEXITSTATUS(status);
}
