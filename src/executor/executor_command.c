/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_command.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caide-so <caide-so@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 15:56:30 by caide-so          #+#    #+#             */
/*   Updated: 2025/05/15 15:58:04 by caide-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	exec_dispatch(char **args, t_env *env, char **envp)
{
	int		status;
	pid_t	pid;

	(void)env;
	//if (is_builtin(args[0]))
		//return (run_builtin(args, env));
	pid = fork();
	if (pid < 0)
		exit_perror("fork failed");
	if (pid == 0)
		execve_with_path(args, env, envp);
	if (waitpid(pid, &status, 0) < 0)
		exit_perror("waitpid failed");
	return (WEXITSTATUS(status));
}

void	execve_with_path(char **args, t_env *env, char **envp)
{
	char	**dirs;
	char	*path_value;
	char	*candidate;
	int		i;

	try_exec_explicit(args[0], args, envp);
	path_value = get_env_path(env);
	dirs = ft_split(path_value, ':');
	if (!dirs)
		exit_perror("split PATH failed");
	i = 0;
	while (dirs[i])
	{
		candidate = ft_strjoin3(dirs[i], "/", args[0]);
		execve(candidate, args, envp);
		free(candidate);
		if (errno != ENOENT)
			perror("execve failed");
		i++;
	}
	free_string_array(dirs);
	perror("command not found");
	exit(127);
}

void	try_exec_explicit(char *cmd, char **args, char **envp)
{
	if (ft_strchr(cmd, '/') != NULL)
	{
		execve(cmd, args, envp);
		perror("execve failed");
		exit(127);
	}
}
