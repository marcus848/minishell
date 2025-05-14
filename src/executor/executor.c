/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caide-so <caide-so@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 21:46:29 by caide-so          #+#    #+#             */
/*   Updated: 2025/05/14 16:17:55 by caide-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	exec_command(t_command *cmd, t_env *env);
char	**env_list_to_array(t_env *env);

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
	*/
	return ;
}

void	exec_command(t_command *cmd, t_env *env)
{
	char	**args;
	char	**envp;
	int	status;
	int save_stdin;
	int save_stdout;

	args = cmd->args;
	save_fds(&save_stdin, &save_stdout);
	handle_redirections(cmd);
	envp = env_list_to_array(env);
	if (!envp)
		exit_perror("env array failed");
	status = exec_dispatch(args, env, envp);
	/*
	else
	{
		pid_t	pid = fork();
		if (pid < 0)
			exit_perror("fork failed");
		else if (pid == 0)
		{
			execve(args[0], args, envp);
			perror("execve failed");
			exit(1);
		}
		else
			if (waitpid(pid, &status, 0) < 0)
				exit_perror("waitpid failed");
	}
	*/
	free_string_array(envp);
	restore_fds(save_stdin, save_stdout);
	set_last_status(status);
}

char	**env_list_to_array(t_env *env)
{
	int		count;
	int		i;
	char	**arr;
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	arr = (char **)malloc((count + 1) * sizeof(char *));
	if (!arr)
		return (NULL);
	tmp = env;
	i = 0;
	while (tmp)
	{
		arr[i] = ft_strjoin3(tmp->key, "=", tmp->value);
		tmp = tmp->next;
		i++;
	}
	arr[count] = NULL;
	return (arr);
}
