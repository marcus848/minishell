/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caide-so <caide-so@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 11:41:48 by caide-so          #+#    #+#             */
/*   Updated: 2025/04/23 11:44:35 by caide-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Future garbage collector, for now only cleans the t_env struct
void	clean_all(t_env *env)
{
	env_free_all(&env);
}

// Free all nodes in the t_env linked list.
void	env_free_all(t_env **head)
{
	t_env	*next;

	while (*head)
	{
		next = (*head)->next;
		free((*head)->key);
		free((*head)->value);
		free(*head);
		*head = next;
	}
}

// Prints perror(msg) and exit with failure code
void	exit_perror(const char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

// Reports a syntax error
void	report_syntax_error(const char *msg)
{
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
}
