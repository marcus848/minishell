/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caide-so <caide-so@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 12:11:05 by caide-so          #+#    #+#             */
/*   Updated: 2025/04/30 20:02:39 by caide-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Prints the full t_envp struct.
void	print_env(t_env *env)
{
	t_env	*head;

	head = env;
	printf("======================== PRINT T_ENV ====================\n\n");
	while (head)
	{
		printf("key = [%s], value = [%s]\n", head->key, head->value);
		head = head->next;
	}
}

void	print_envp(char **envp)
{
	int	i;

	i = 0;
	printf("======================== PRINT ENVP ====================\n\n");
	while (envp[i])
	{
		printf("%s\n", envp[i]);
		i++;
	}
}

// Print the token and its value

void	print_tokens(t_token_list *tokens)
{
	t_token	*token;

	token = tokens->head;
	while (token)
	{
		print_token(NULL, token);
		token = token->next;
	}
}

void	print_token(char *str_type, t_token *token)
{
	if (token->type == WORD)
		str_type = "WORD";
	else if (token->type == PIPE)
		str_type = "PIPE";
	else if (token->type == REDIR_IN)
		str_type = "REDIR_IN";
	else if (token->type == REDIR_OUT)
		str_type = "REDIR_OUT";
	else if (token->type == REDIR_APPEND)
		str_type = "REDIR_APPEND";
	else if (token->type == HEREDOC)
		str_type = "HEREDOC";
	else if (token->type == LOGICAL_AND)
		str_type = "LOGICAL_AND";
	else if (token->type == LOGICAL_OR)
		str_type = "LOGICAL_OR";
	else if (token->type == PAREN_OPEN)
		str_type = "PAREN_OPEN";
	else if (token->type == PAREN_CLOSE)
		str_type = "PAREN_CLOSE";
	printf("type - %s, value - [%s]\n", str_type, token->value);
}
