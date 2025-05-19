/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caide-so <caide-so@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 02:45:39 by caide-so          #+#    #+#             */
/*   Updated: 2025/05/19 04:12:40 by caide-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_all_n_flag(const char *str);

int	builtin_echo(char **args)
{
	int	supress_nl;
	int	i;

	supress_nl = 0;
	i = 1;
	while (args[i] && is_all_n_flag(args[i]))
	{
		supress_nl = 1;
		i++;
	}
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (!supress_nl)
		printf("\n");
	return (0);
}

int	is_all_n_flag(const char *str)
{
	int	i;

	if (str[0] != '-' || str[1] == '\0')
		return (0);
	i = 1;
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}
