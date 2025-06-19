/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caide-so <caide-so@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 01:57:27 by caide-so          #+#    #+#             */
/*   Updated: 2025/05/19 02:31:12 by caide-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Builtin impltmentation for the "pwd" command.
// 1. Attempts to get the PWD value from environment.
// 2. Prints PWD if found.
// 3. Otherwise, calls getcwd to get current working directory.
// 4. Prints current directory or error message on failure.
// 5. Frees allocated memory if getcwd is used.
int	builtin_pwd(t_env *env)
{
	char	*pwd;

	pwd = get_env_value(env, "PWD");
	if (pwd)
	{
		printf("%s\n", pwd);
		return (0);
	}
	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		perror("pwd");
		return (1);
	}
	printf("%s\n", pwd);
	free(pwd);
	return (0);
}
