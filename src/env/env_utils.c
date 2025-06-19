/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caide-so <caide-so@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 10:55:50 by caide-so          #+#    #+#             */
/*   Updated: 2025/06/17 02:09:48 by caide-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	env_create_append(t_env **env_head, char *key, char *val, t_env *curr);

// Retrieves value for environment variable key.
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

// Gets PATH environment variable value.
char	*get_env_path(t_env *env)
{
	char	*path;

	path = get_env_value(env, "PATH");
	return (path);
}

// Updates existing environment variable or creates new.
// 1. Finds and updates matching key.
// 2. Creates new entry if key was not found.
void	env_update(t_env **env_head, char *key, char *value)
{
	t_env	*curr;

	curr = *env_head;
	while (curr)
	{
		if (ft_strcmp(curr->key, key) == 0)
		{
			free(curr->value);
			curr->value = ft_strdup(value);
			return ;
		}
		curr = curr->next;
	}
	env_create_append(env_head, key, value, curr);
}

// Creates and appends new environment variable.
void	env_create_append(t_env **env_head, char *key, char *val, t_env *curr)
{
	t_env	*node;

	node = env_new(key, val);
	if (!node)
		exit_perror("env_new failed");
	if (*env_head == NULL)
		*env_head = node;
	else
	{
		curr = *env_head;
		while (curr->next)
			curr = curr->next;
		curr->next = node;
	}
}
