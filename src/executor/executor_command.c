/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_command.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caide-so <caide-so@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 21:00:59 by caide-so          #+#    #+#             */
/*   Updated: 2025/06/13 04:03:01 by caide-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	try_exit(t_token_list *toks, char **args, t_shell *shell)
{
	if (!args || !args[0])
		return (1);
	if (ft_strcmp(args[0], "exit") == 0)
		builtin_exit(toks, shell);
	return (0);
}

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

void	run_external_cmd(char **args, t_command *cmd, t_shell *shell)
{
	if (is_explicit_dir(args[0]))
	{
		handle_dir_case(args[0], shell);
		return ;
	}
	if (!is_executable_command(args[0], shell->env))
	{
		handle_not_found(args[0], shell);
		return ;
	}
	exec_with_redirs(args, cmd, shell);
}
