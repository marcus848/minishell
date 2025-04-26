/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caide-so <caide-so@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 12:11:05 by caide-so          #+#    #+#             */
/*   Updated: 2025/04/25 12:17:31 by caide-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Prints the full t_envp struct.
void	print_env(t_env *env)
{
	t_env	*head;

	head = env;
	while (head)
	{
		printf("key = [%s], value = [%s]\n", head->key, head->value);
		head = head->next;
	}
}

// Print the token and its value

void	print_tokens(t_token_list *tokens)
{
	t_token	*token;

	token = tokens->head;
	while (token)
	{
		print_token(token);
		token = token->next;
	}
}

void	print_token(t_token *token)
{
	char	*str_type;

	str_type = NULL;
	if (token->type == 0)
		str_type = "WORD";
	else if (token->type == 1)
		str_type = "PIPE";
	else if (token->type == 2)
		str_type = "REDIR_IN";
	else if (token->type == 3)
		str_type = "REDIR_OUT";
	else if (token->type == 4)
		str_type = "REDIR_APPEND";
	else if (token->type == 5)
		str_type = "HEREDOC";
	else if (token->type == 6)
		str_type = "LOGICAL_AND";
	else if (token->type == 7)
		str_type = "LOGICAL_OR";
	else if (token->type == 8)
		str_type = "PAREN_OPEN";
	else if (token->type == 9)
		str_type = "PAREN_CLOSE";
	else if (token->type == 10)
		str_type = "ASTERISK";
	printf("type - %s, value - [%s]\n", str_type, token->value);
}
