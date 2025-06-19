/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_expansion.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcudos <marcudos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 16:22:33 by marcudos          #+#    #+#             */
/*   Updated: 2025/05/20 21:57:46 by marcudos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Frees a null-terminated array of pointers.
// 1. Frees each element in the array.
// 2. Frees the array itself.
void	free_array(void **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

// Frees a null-terminated array of strings.
// 1. Frees each string in the array.
// 2. Frees the array itself.
// Note: Specialized for char ** arrays.
void	free_args_temp(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		free(args[i++]);
	free(args);
}

// Frees a linked list of argument nodes.
// 1. Frees each string in the list.
// 2. Frees each list node.
void	free_args_list(t_args *args)
{
	t_args	*temp;

	while (args)
	{
		temp = args;
		args = args->next;
		free(temp->arg);
		free(temp);
	}
}
