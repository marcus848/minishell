/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caide-so <caide-so@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 18:55:47 by caide-so          #+#    #+#             */
/*   Updated: 2025/04/25 12:22:50 by caide-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Initializes t_token list struct.
void	token_list_init(t_token_list *list)
{
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
}

// Creates a new token.
// 1. Allocates a t_token.
// 2. Sets type and value.
// 3. Initializes next to NULL.
t_token	*new_token(t_token_type type, char *value)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = value;
	token->next = NULL;
	return (token);
}

// Appends token to the end of the list.
// 1. Links new token to tail if list is not empty.
// 2. Updates head if list was empty.
// 3. Updates tail and increments size.
void	token_list_append(t_token_list *list, t_token *token)
{
	if (list->tail)
		list->tail->next = token;
	else
		list->head = token;
	list->tail = token;
	list->size++;
}

// Frees all tokens in the list and the list itself.
// 1. Iterates through each token.
// 2. Frees token value and token struct.
// 3. Frees the list structure.
void	token_list_free(t_token_list *list)
{
	t_token	*curr;
	t_token	*next;

	if (!list)
		return ;
	curr = list->head;
	while (curr)
	{
		next = curr->next;
		if (curr->value)
		{
			free(curr->value);
			curr->value = NULL;
		}
		free(curr);
		curr = next;
	}
	free(list);
	list = NULL;
}
