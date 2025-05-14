/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caide-so <caide-so@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 21:46:29 by caide-so          #+#    #+#             */
/*   Updated: 2025/05/13 21:49:14 by caide-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	exec_command(t_command *cmd, t_env *env);
void	handle_redirections(t_command *cmd);

/*
void	executor(t_ast *node, t_env *env)
{
	if (!node)
		return ;
	// traverse ast
	if (node->type == NODE_COMMAND)
		// wildcard expansion
 		// variable expansion
 		// handle redirections
 		// execute builtin or fork-exec
 		// restore redirections
		exec_command(node->cmd, env);
	if (node->type == NODE_PIPE)
		// create pipe
		// fork left, redirect stdout
		// fork right, redirect stdin
		// wait and update status
		// obs: inside exec_pipe fork left/right and maybe recurse there
		exec_pipe(node, env);
	if (node->type == NODE_AND)
	{
	 	// execute left, test status, maybe execute right
		executor(node->left, env);
		if (get_last_status() == 0)
			executor(node->right, env);
	}
	if (node->type == NODE_OR)
	{
	 	// execute left, test status, maybe execute right
		executor(node->left, env);
		if (get_last_status() != 0)
			executor(node->right, env);
	}
	if (node->type == NODE_SUBSHELL)
		// fork, apply redirs, exec subtree, wait and update status
		// obs: subshell must execute in a child process
		executor(node->left, env);
	return ;
}

void	exec_command(t_command *cmd, t_env *env)
{
	char	**args;
	int	status;
	int save_fd_stdin;
	int save_fd_stdout;

	args = cmd->args;
	envp = to_envp(env);
	// handle redirections
	save_fd_stdin = dup(STDIN_FILENO);
	save_fd_stdout = dup(STDOUT_FILENO);
	handle_redirections(cmd);
	// Check for builtin
	if (is_builtin(args[0]))
	{
		status = run_builtin(args, env);
	}
	else
	{
		// Fork + exec
		pid_t	pid = fork();
		if (pid == 0)
		{
			execve(args[0], args, envp);
			perror("execve failed");
			exit(1);
		}
		else
			waitpid(pid, &status, 0);
	}
	// Restore original stdio
	dup2(save_fd_stdin, STDIN_FILENO);
	dup2(save_fd_stdout, STDOUT_FILENO);
	close(save_fd_stdin);
	close(save_fd_stdout);
	// Update last status
	set_last_status(status);
}

void	handle_redirections(t_command *cmd)
{
	int	fd_in;
	int	fd_out;
	int	fd_app;
	int	fd_here;

	// Handle redirections
	if (cmd->infile)
	{
		fd_in = open(cmd->infile, O_RDONLY);
		dup2(fd_in, STDIN_FILENO);
		close(fd_in);
	}
	if (cmd->outfile)
	{
		fd_out = open(cmd->outfile, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		dup2(fd_out, STDOUT_FILENO);
		close(fd_out);
	}
	if (cmd->appendfile)
	{
		fd_app = open(cmd->appendfile, O_CREAT | O_WRONLY | O_APPEND, 0644);
		dup2(fd_app, STDOUT_FILENO);
		close(fd_app);
	}
	if (cmd->heredoc)
	{
		fd_here = open(cmd->heredoc_path, O_RDONLY);
		dup2(fd_here, STDOUT_FILENO);
		close(fd_here);
	}
}
*/
