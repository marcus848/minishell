/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_command.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caide-so <caide-so@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 21:00:59 by caide-so          #+#    #+#             */
/*   Updated: 2025/05/20 21:01:12 by caide-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	try_exit_builtin(t_token_list *toks, t_ast *node, t_env *env)
{
	char	*cmd;

	cmd = node->cmd->args[0];
	if (ft_strcmp(cmd, "exit") == 0)
		builtin_exit(toks, node, env);
	return (0);
}

int	try_other_builtin(char **args, t_env *env)
{
	int	status;

	if (is_builtin(args[0]))
	{
		status = run_builtin(args, env);
		set_last_status(env, status);
		return (1);
	}
	return (0);
}

void	run_external_command(char **args, t_command *cmd, t_env *env)
{
	char	**envp;
	int		status;
	int		save_stdin;
	int		save_stdout;

	if (!is_executable_command(args[0], env))
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
