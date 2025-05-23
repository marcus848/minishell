/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caide-so <caide-so@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 18:32:16 by caide-so          #+#    #+#             */
/*   Updated: 2025/05/23 18:43:37 by caide-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

size_t	env_count(t_env *env);
t_env	**env_to_array(t_env *env, size_t n);
void	sort_env_array(t_env **arr, size_t n);
void	print_env_array(t_env **arr, size_t n);

void	print_exported(t_env *env)
{
	size_t	n;
	t_env	**arr;

	n = env_count(env);
	if (n == 0)
		return ;
	arr = env_to_array(env, n);
	if (!arr)
		return ;
	sort_env_array(arr, n);
	print_env_array(arr, n);
	free(arr);
}

size_t	env_count(t_env *env)
{
	size_t	count;
	t_env	*tmp;

	count = 0;
	tmp = env;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	return (count);
}

t_env	**env_to_array(t_env *env, size_t n)
{
	t_env	**arr;
	size_t	i;
	t_env	*tmp;

	arr = (t_env **)malloc(sizeof(t_env *) * n);
	if (!arr)
		return (NULL);
	tmp = env;
	i = 0;
	while (i < n)
	{
		arr[i] = (t_env *)tmp;
		tmp = tmp->next;
		i++;
	}
	return (arr);
}

void	sort_env_array(t_env **arr, size_t n)
{
	size_t	i;
	size_t	j;
	t_env	*tmp;

	i = 0;
	while (i < n)
	{
		j = i + 1;
		while (j < n)
		{
			if (ft_strcmp(arr[i]->key, arr[j]->key) > 0)
			{
				tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

void	print_env_array(t_env **arr, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putstr_fd(arr[i]->key, STDOUT_FILENO);
		ft_putstr_fd("=\"", STDOUT_FILENO);
		ft_putstr_fd(arr[i]->value, STDOUT_FILENO);
		ft_putstr_fd("\"\n", STDOUT_FILENO);
		i++;
	}
}
