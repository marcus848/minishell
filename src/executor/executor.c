/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caide-so <caide-so@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 21:46:29 by caide-so          #+#    #+#             */
/*   Updated: 2025/05/26 03:12:22 by caide-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	exec_command(t_token_list *tokens, t_shell *shell, t_ast *node);
void	exec_pipe(t_shell *shell, t_ast *node);
void	exec_subshell(t_token_list *tokens, t_shell *shell, t_ast *node);

/*
if (node->type == NODE_COMMAND)
{
	// variable expansion
	// wildcard expansion
 	// handle redirections
 	// execute builtin or fork-exec
 	// restore redirections
	expander(&node->cmd->args, sh->env, &node->cmd->arg_count);
	exec_command(tokens, sh);
}
else if (node->type == NODE_PIPE)
	// create pipe
	// fork left, redirect stdout
	// fork right, redirect stdin
	// wait and update status
	// obs: inside exec_pipe fork left/right and maybe recurse there
	exec_pipe(sh);
else if (node->type == NODE_AND)
{
	// execute left, test status, maybe execute right
	executor(tokens, sh, node->left);
	if (get_last_status(sh) == 0)
		executor(tokens, sh, node->right);
}
else if (node->type == NODE_OR)
{
	// execute left, test status, maybe execute right
	executor(tokens, sh, node->left);
	if (get_last_status(sh) != 0)
		executor(tokens, sh, node->right);
}
else if (node->type == NODE_SUBSHELL)
	// fork, apply redirs, exec subtree, wait and update status
	// obs: subshell must execute in a child process
	executor(tokens, sh, node->left);
*/
void	executor(t_token_list *tokens, t_shell *sh, t_ast *node)
{
	if (!node)
		return ;
	if (node->type == NODE_COMMAND)
	{
		expander(&node->cmd->args, sh->env, &node->cmd->arg_count);
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
		if (get_last_status(sh) != 0)
			executor(tokens, sh, node->right);
	}
	else if (node->type == NODE_SUBSHELL)
		exec_subshell(tokens, sh, node->left);
	return ;
}

void	exec_command(t_token_list *tokens, t_shell *shell, t_ast *node)
{
	char	**args;

	args = node->cmd->args;
	if (try_exit(tokens, args, shell))
		return ;
	if (try_other_builtin(args, shell))
		return ;
	run_external_cmd(args, node->cmd, shell);
}

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

// Child: execute the subtree, then exit with its status
// Grab the last status from the shell context
// Parent: wait for the child and update last_status
void	exec_subshell(t_token_list *tokens, t_shell *shell, t_ast *node)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
		exit_perror("fork");
	if (pid == 0)
	{
		executor(tokens, shell, node);
		clean_all(shell->tokens, shell->ast, &shell->env);
		exit(shell->last_status);
	}
	if (waitpid(pid, &status, 0) < 0)
		exit_perror("waitpid");
	shell->last_status = WEXITSTATUS(status);
}
