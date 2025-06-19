/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_command.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caide-so <caide-so@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 21:00:59 by caide-so          #+#    #+#             */
/*   Updated: 2025/06/16 00:27:29 by caide-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	handle_permission_denied(char *cmd, t_shell *shell);

// Checks if command is "exit" and runs builtin_exit if so.
// 1. Returns 1 if args is NULL or "exit" is detected.
// 2. Calls builtin_exit with toks and shell if "exit" is matched.
// 3. Returns 0 otherwise.
int	try_exit(t_token_list *toks, char **args, t_shell *shell)
{
	if (!args || !args[0])
		return (1);
	if (ft_strcmp(args[0], "exit") == 0)
		builtin_exit(toks, shell);
	return (0);
}

// Runs built-in command with redirection if applicable.
// 1. Checks if command is a builtin.
// 2. Saves current stdin and stdout.
// 3. Applies redirection via apply_redirections().
// 4. Restores original fds if redirectoin fails.
// 5. Runs builtin and restores fds.
// 6. Updates last exit status.
// 7. Returns 1 if builtin was executed, 0 otherwise.
// Note: Manages redirections and builtin lifecycle.
int	try_other_builtin(char **args, t_command *cmd, t_shell *shell)
{
	int	status;
	int	save_stdin;
	int	save_stdout;

	if (is_builtin(args[0]))
	{
		save_fds(&save_stdin, &save_stdout);
		if (apply_redirections(cmd, shell) < 0)
		{
			restore_fds(save_stdin, save_stdout);
			return (1);
		}
		status = run_builtin(args, shell);
		restore_fds(save_stdin, save_stdout);
		set_last_status(shell, status);
		return (1);
	}
	return (0);
}

// Executes external command if it is valid and executable.
// 1. If path is an explicit directory, handles it with error.
// 2. Checks if command is executable.
// 3. Executes command if valid.
// 4. Handles permission denied or not found cases.
// Note: Dispatches final execution logic for non-builtins.
void	run_external_cmd(char **args, t_command *cmd, t_shell *shell)
{
	int	exec_status;

	if (is_explicit_dir(args[0]))
	{
		handle_dir_case(args[0], shell);
		return ;
	}
	exec_status = is_executable_command(args[0], shell->env);
	if (exec_status == 1)
		exec_with_redirs(args, cmd, shell);
	else if (exec_status == -1)
		handle_permission_denied(args[0], shell);
	else
		handle_not_found(args[0], shell);
}
