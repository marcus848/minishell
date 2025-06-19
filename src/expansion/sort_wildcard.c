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

// Swaps the 'arg' strings between two t_args nodes.
// 1. Temporarily stores a->arg.
// 2. Assigns b->arg to a->arg.
// 3. Assigns temp to b->arg.
void	swap_args(t_args *a, t_args *b)
{
	char	*temp;

	temp = a->arg;
	a->arg = b->arg;
	b->arg = temp;
}

// Converts uppercase character to lowercase.
// 1. Returns lowercase equivalent if character is in 'A'-'Z'.
// 2. Otherwise, returns character unchanged.
int	ft_tolower(int c)
{
	if (c >= 'A' && c <= 'Z')
		return (c + 32);
	return (c);
}

// Compares two strings ignoring case.
// 1. Iterates through both strings.
// 2. Compares characters after converting to lowercase.
// 3. Returns difference on first mismatch, or 0 if equal.
// Note: Case-insensitive version of ft_strcmp().
int	ft_strcasecmp(const char *s1, const char *s2)
{
	unsigned char	c1;
	unsigned char	c2;

	while (*s1 || *s2)
	{
		c1 = ft_tolower((int) *s1);
		c2 = ft_tolower((int) *s2);
		if (c1 != c2)
			return (c1 - c2);
		s1++;
		s2++;
	}
	return (0);
}

// Sorts a t_args linked list in ascending order (case-insensitive).
// 1. Iteratively compares adjacent nodes.
// 2. Swaps arguments if out of order.
// 3. Repeats until no swaps occur.
// Note: Uses bubble sort, suitable for small lists.
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
			if (ft_strcasecmp(cur->arg, next->arg) > 0)
			{
				swap_args(cur, next);
				swapped = 1;
			}
			cur = cur->next;
		}
	}
}

// Sorts a list and appends it to the head list.
// 1. Sorts the list using sort_args_list.
// 2. Appends the sorted list using append_list.
void	sort_append(t_args **head, t_args *list)
{
	if (!list)
		return ;
	sort_args_list(list);
	append_list(head, list);
}
