/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcudos <marcudos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 16:53:08 by marcudos          #+#    #+#             */
/*   Updated: 2025/05/20 21:07:50 by marcudos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	add_token(t_args **head, char *value)
{
	t_args	*new;
	t_args	*temp;

	if (!value)
		return ;
	new = (t_args *)malloc(sizeof(t_args));
	if (!new)
		return ;
	new->arg = ft_strdup(value);
	new->next = NULL;
	if (!(*head))
		*head = new;
	else
	{
		temp = *head;
		while (temp->next)
			temp = temp->next;
		temp->next = new;
	}
}
