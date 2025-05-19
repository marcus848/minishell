/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caide-so <caide-so@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 22:30:29 by caide-so          #+#    #+#             */
/*   Updated: 2025/05/19 01:47:16 by caide-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	builtin_exit(t_token_list *tokens, t_ast *node, t_env *env)
{
	int		code;
	char	*arg_str;

	if (node->cmd->args[2])
	{
		if (node->cmd->args[2][0])
		{
			ft_putstr_fd("exit\nminishell: exit: ", STDERR_FILENO);
			ft_putstr_fd("too many arguments\n", STDERR_FILENO);
			clean_all(tokens, node, env);
			exit(1);
		}
	}
	arg_str = node->cmd->args[1];
	code = parse_exit_code(arg_str, env);
	clean_all(tokens, node, env);
	if (code == -1)
		exit(2);
	exit(code);
}

// Check that arg_str consistis only of digits
// (and optional leading +/-)
// 	if not, print an error ("numeric argument required") and exit
// 	with status 2
int	parse_exit_code(char *arg_str, t_env *env)
{
	int	code;
	int	i;

	if (!arg_str)
		return (get_last_status(env));
	i = 0;
	if (arg_str[i] == '+' || arg_str[i] == '-')
		i++;
	while (arg_str[i])
	{
		if (!ft_isdigit(arg_str[i]))
		{
			ft_putstr_fd("exit\nminishell: exit: ", STDERR_FILENO);
			ft_putstr_fd(arg_str, STDERR_FILENO);
			ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
			return (-1);
		}
		i++;
	}
	ft_putstr_fd("exit\n", STDERR_FILENO);
	code = ft_atoi(arg_str);
	return (code & 0xFF);
}
