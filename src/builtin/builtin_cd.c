/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caide-so <caide-so@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 21:03:47 by caide-so          #+#    #+#             */
/*   Updated: 2025/05/19 22:15:01 by caide-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*get_cd_target(char **args, t_env *env);
static char	*do_chdir(const char *path, t_env *env);
static void	update_env_vars(t_env *env, char *oldcwd);

int	builtin_cd(char **args, t_env *env)
{
	char	*target;
	char	*oldcwd;

	target = get_cd_target(args, env);
	if (!target)
		return (1);
	oldcwd = do_chdir(target, env);
	if (!oldcwd)
		return (1);
	update_env_vars(env, oldcwd);
	return (0);
}

static char	*get_cd_target(char **args, t_env *env)
{
	char	*home;	

	if (!args[1])
	{
		home = get_env_value(env, "HOME");
		if (!home)
		{
			ft_putstr_fd("cd: HOME not set\n", STDERR_FILENO);
			return (NULL);
		}
		return (home);
	}
	if (args[2])
	{
		ft_putstr_fd("cd: too many arguments\n", STDERR_FILENO);
		return (NULL);
	}
	return (args[1]);
}

static char	*do_chdir(const char *path, t_env *env)
{
	char	*oldpwd_env;
	char	*oldcwd;

	oldpwd_env = get_env_value(env, "PWD");
	if (oldpwd_env)
		oldcwd = ft_strdup(oldpwd_env);
	else
		oldcwd = getcwd(NULL, 0);
	if (!oldcwd)
		return (NULL);
	if (chdir(path) < 0)
	{
		perror("cd");
		free(oldcwd);
		return (NULL);
	}
	return (oldcwd);
}

static void	update_env_vars(t_env *env, char *oldcwd)
{
	char	*newcwd;

	env_update(&env, "OLDPWD", oldcwd);
	free(oldcwd);
	newcwd = getcwd(NULL, 0);
	if (newcwd)
	{
		env_update(&env, "PWD", newcwd);
		free(newcwd);
	}
}
