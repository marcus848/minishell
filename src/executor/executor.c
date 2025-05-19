/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caide-so <caide-so@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 21:46:29 by caide-so          #+#    #+#             */
/*   Updated: 2025/05/18 23:03:23 by caide-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	exec_command(t_command *cmd, t_env *env);

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
	/*
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
		if (get_last_status(env) == 0)
			executor(node->right, env);
	}
	if (node->type == NODE_OR)
	{
	 	// execute left, test status, maybe execute right
		executor(node->left, env);
		if (get_last_status(env) != 0)
			executor(node->right, env);
	}
	if (node->type == NODE_SUBSHELL)
		// fork, apply redirs, exec subtree, wait and update status
		// obs: subshell must execute in a child process
		executor(node->left, env);
	*/
	return ;
}

void	exec_command(t_command *cmd, t_env *env)
{
	char	**args;
	char	**envp;
	int		status;
	int		save_stdin;
	int		save_stdout;

	args = cmd->args;
	if (ft_strcmp(cmd->args[0], "exit") == 0)
		run_builtin(cmd->args, env);
	if (!is_executable_command(cmd->args[0], env))
	{
		ft_putstr_fd(args[0], STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		set_last_status(env, 127);
		return ;
	}
	save_fds(&save_stdin, &save_stdout);
	handle_redirections(cmd);
	envp = env_list_to_array(env);
	if (!envp)
		exit_perror("env array failed");
	status = exec_dispatch(args, env, envp);
	free_string_array(envp);
	restore_fds(save_stdin, save_stdout);
	set_last_status(env, status);
}
