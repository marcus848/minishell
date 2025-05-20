/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcudos <marcudos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 16:14:44 by marcudos          #+#    #+#             */
/*   Updated: 2025/05/19 16:53:37 by marcudos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	expander(char ***args, t_env *env, int *size_args)
{
	t_args	*head;
	char		**expanded;

	if (!(*args) || !(*args)[0])
		return ;
	head = expand_env(*args, env);
	expanded = list_to_args(head, size_args);
	free_args_temp(*args);
	(*args) = expanded;
	free_args_list(head);
}

char	**list_to_args(t_args *args, int *size_args)
{
	t_args	*temp;
	char	**res;
	int		size;
	int		i;

	temp = args;
	size = 0;
	while (temp)
	{
		size++;
		temp = temp->next;
	}
	*size_args = size;
	res = (char **) malloc(sizeof(char *) * (size + 1));
	if (!res)
		return (NULL);
	i = 0;
	while (args)
	{
		res[i] = ft_strdup(args->arg);
		i++;
		args = args->next;
	}
	res[i] = NULL;
	return (res);
}

t_args	*args_to_list(char **args)
{
	t_args	*head;
	t_args	*arg;
	t_args	*new;
	int		i;

	i = 0;
	while (args[i])
	{
		if (i == 0)
		{
			head = (t_args *) malloc(sizeof(t_args));
			head->arg = ft_strdup(args[i]);
			head->next = NULL;
			arg = head;
		}
		else
		{
			new = (t_args *) malloc(sizeof(t_args));
			new->arg = ft_strdup(args[i]);
			new->next = NULL;
			arg->next = new;
			arg = arg->next;
		}
		i++;
	}
	return (head);
}
