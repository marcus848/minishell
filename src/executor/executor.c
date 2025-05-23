/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caide-so <caide-so@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 21:46:29 by caide-so          #+#    #+#             */
/*   Updated: 2025/05/23 03:40:31 by caide-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	exec_command(t_token_list *tokens, t_shell *shell);

/*
if (node->type == NODE_COMMAND)
{
	// variable expansion
	// wildcard expansion
 	// handle redirections
 	// execute builtin or fork-exec
 	// restore redirections
	expander(&node->cmd->args, env, &node->cmd->arg_count);
	exec_command(tokens, node, env);
}
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
void	executor(t_token_list *tokens, t_shell *sh)
{
	if (!sh->ast)
		return ;
	if (sh->ast->type == NODE_COMMAND)
	{
		expander(&sh->ast->cmd->args, sh->env, &sh->ast->cmd->arg_count);
		exec_command(tokens, sh);
	}
	return ;
}

void	exec_command(t_token_list *tokens, t_shell *shell)
{
	char	**args;

	args = shell->ast->cmd->args;
	if (try_exit_builtin(tokens, shell))
		return ;
	if (try_other_builtin(args, shell))
		return ;
	run_external_cmd(args, shell->ast->cmd, shell);
}
