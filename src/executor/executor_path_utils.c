/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_path_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caide-so <caide-so@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 23:00:32 by caide-so          #+#    #+#             */
/*   Updated: 2025/06/16 00:24:13 by caide-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Checks if command is an executable file.
// 1. If command contains '/', checks if file exists, is not a directory, and
// is executable.
// 2. Otherwise, searches in PATH via search_in_path().
// 3. Returns 1 if executable, 0 if not found or not executable, -1 if
// permission denied.
// Note: Used to validate command before execve.
int	is_executable_command(char *cmd, t_env *env)
{
	struct stat	sb;

	if (is_explicit_executable(cmd))
	{
		if (access(cmd, F_OK) != 0)
			return (0);
		if (stat(cmd, &sb) == 0 && S_ISDIR(sb.st_mode))
			return (0);
		if (access(cmd, X_OK) == 0)
			return (1);
		return (-1);
	}
	return (search_in_path(cmd, env));
}

// Checks if command is an explicit path.
// 1. Returns 1 if command contains '/'.
// 2. Returns 0 otherwise.
// Note: Distinguishes between absolute/relative and bare commands.
int	is_explicit_executable(char *cmd)
{
	if (ft_strchr(cmd, '/') == NULL)
		return (0);
	return (1);
}

// Searches PATH to check if command is executable.
// 1. Splits PATH into directoreis.
// 2. Appends command to each directory.
// 3. Uses access to check if candidate is executable.
// Returns 1 on success, 0 otherwise.
int	search_in_path(char *cmd, t_env *env)
{
	char	**dirs;
	char	*full;
	int		i;

	dirs = ft_split(get_env_path(env), ':');
	if (!dirs)
		return (0);
	i = 0;
	while (dirs[i])
	{
		full = ft_strjoin3(dirs[i], "/", cmd);
		if (!full)
			break ;
		if (access(full, X_OK) == 0)
		{
			free(full);
			free_string_array(dirs);
			return (1);
		}
		free(full);
		i++;
	}
	free_string_array(dirs);
	return (0);
}
