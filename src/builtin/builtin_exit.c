/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caide-so <caide-so@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 22:30:29 by caide-so          #+#    #+#             */
/*   Updated: 2025/05/23 03:30:59 by caide-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Builtin implementation for the "exit" command.
// 1. Checks for too many arguments and errors out if present.
// 2. Parses the optional exit cod argument.
// 3. Frees all allocated resources.
// 4. Exits with the parsed code on error code.
// Note: Exits with status 2 on invalid numeric argument.
void	builtin_exit(t_token_list *tokens, t_shell *shell)
{
	int		code;
	char	*arg_str;

	if (shell->ast->cmd->args[2])
	{
		if (shell->ast->cmd->args[2][0])
		{
			ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
			ft_putstr_fd("too many arguments\n", STDERR_FILENO);
			clean_all(tokens, shell->ast, &shell->env);
			exit(1);
		}
	}
	arg_str = shell->ast->cmd->args[1];
	code = parse_exit_code(arg_str, shell);
	clean_all(tokens, shell->ast, &shell->env);
	if (code == -1)
		exit(2);
	exit(code);
}

// Parses and validates the exit code argument.
// 1. Returns last shell status if no argument.
// 2. Validates the string contains only digits (with optional +/-).
// 3. Prints error and returns -1 on invalid input.
// 4. Returns code masked to 8 bits.
int	parse_exit_code(char *arg_str, t_shell *shell)
{
	int	code;
	int	i;

	if (!arg_str)
		return (get_last_status(shell));
	i = 0;
	if (arg_str[i] == '+' || arg_str[i] == '-')
		i++;
	while (arg_str[i])
	{
		if (!ft_isdigit(arg_str[i]))
		{
			ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
			ft_putstr_fd(arg_str, STDERR_FILENO);
			ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
			return (-1);
		}
		i++;
	}
	code = ft_atoi(arg_str);
	return (code & 0xFF);
}
