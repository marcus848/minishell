/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caide-so <caide-so@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 10:55:50 by caide-so          #+#    #+#             */
/*   Updated: 2025/05/19 22:14:13 by caide-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*get_env_value(t_env *env, const char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

char	*get_env_path(t_env *env)
{
	char	*path;

	path = get_env_value(env, "PATH");
	if (path && *path != '\0')
		return (path);
	return ("/usr/local/bin:/usr/bin:/bin");
}

/*
void	update_env(t_env *env, const char *key, const char *value)
{
	
}
*/
