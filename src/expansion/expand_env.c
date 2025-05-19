/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcudos <marcudos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 11:22:33 by marcudos          #+#    #+#             */
/*   Updated: 2025/05/19 11:31:50 by marcudos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_args	*expand_env(char **args, t_env *env)
{
	t_args	*head;
	t_args	*cur;
	t_args	*new;
	int	i;

	if (!args || !args[0])
		return (NULL);
	head = expand_token(args[0], env);
	cur = head;
	i = 1;
	while (args[i])
	{
		new = expand_token(args[i], env);
		while (cur->next)
			cur = cur->next;
		cur->next = new;
		i++;
	}
	return (head);
}
