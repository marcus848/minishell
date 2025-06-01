/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_wildcard.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcudos <marcudos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 18:43:32 by marcudos          #+#    #+#             */
/*   Updated: 2025/06/01 18:53:09 by marcudos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <readline/history.h>

void	swap_args(t_args *a, t_args *b)
{
	char	*temp;

	temp = a->arg;
	a->arg = b->arg;
	b->arg = temp;
}

void	sort_args_list(t_args *list)
{
	t_args	*cur;
	t_args	*next;
	int		swapped;

	if (!list)
		return ;
	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		cur = list;
		while (cur && cur->next)
		{
			next = cur->next;
			if (ft_strcmp(cur->arg, next->arg) > 0)
			{
				swap_args(cur, next);
				swapped = 1;
			}
			cur = cur->next;
		}
	}
}

void	sort_append(t_args **head, t_args *list)
{
	if (!list)
		return ;
	sort_args_list(list);
	append_list(head, list);
}
