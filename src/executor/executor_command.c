/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_command.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caide-so <caide-so@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 21:00:59 by caide-so          #+#    #+#             */
/*   Updated: 2025/05/23 03:38:27 by caide-so         ###   ########.fr       */
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

int	try_other_builtin(char **args, t_shell *shell)
{
	int	status;

	if (is_builtin(args[0]))
	{
		status = run_builtin(args, shell->env);
		set_last_status(shell, status);
		return (1);
	}
	return (0);
}

void	run_external_cmd(char **args, t_command *cmd, t_shell *sh)
{
	char	**envp;
	int		status;
	int		save_stdin;
	int		save_stdout;

	if (!is_executable_command(args[0], sh->env))
	{
		ft_putstr_fd(args[0], STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		set_last_status(sh, 127);
		return ;
	}
	save_fds(&save_stdin, &save_stdout);
	handle_redirections(cmd);
	envp = env_list_to_array(sh->env);
	if (!envp)
		exit_perror("env array failed");
	status = exec_dispatch(args, sh->env, envp);
	free_string_array(envp);
	restore_fds(save_stdin, save_stdout);
	set_last_status(sh, status);
}
