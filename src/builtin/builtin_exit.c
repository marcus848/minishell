/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caide-so <caide-so@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 22:30:29 by caide-so          #+#    #+#             */
/*   Updated: 2025/05/15 22:55:48 by caide-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
			exit(2);
		}
		else
			ft_putstr_fd("exit\n", STDERR_FILENO);
		i++;
	}
	code = ft_atoi(arg_str);
	return (code & 0xFF);
}
