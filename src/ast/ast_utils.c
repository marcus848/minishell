/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcudos <marcudos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 16:44:40 by marcudos          #+#    #+#             */
/*   Updated: 2025/05/03 17:13:37 by marcudos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Checks if token is a pipe, logical operator, or parenthesis.
// 1. Returns 1 for PIPE, LOGICAL_OR, LOGICAL_AND.
// 2. Returns 1 for PAREN_OPEN or PAREN_CLOSE.
// 3. Returns 0 otherwise.
int	is_pipe_or_logical(t_token *token)
{
	t_token_type	type;

	if (!token)
		return (0);
	type = token->type;
	if (type == PIPE)
		return (1);
	else if (type == LOGICAL_OR || type == LOGICAL_AND)
		return (1);
	else if (type == PAREN_OPEN || type == PAREN_CLOSE)
		return (1);
	return (0);
}

// Checks if token is a redirection operator.
// 1. Returns 1 for redirections and 0 otherwise.
int	is_redirect(t_token *token)
{
	t_token_type	type;

	if (!token)
		return (0);
	type = token->type;
	if (type == REDIR_IN)
		return (1);
	else if (type == REDIR_OUT)
		return (1);
	else if (type == REDIR_APPEND)
		return (1);
	else if (type == HEREDOC)
		return (1);
	return (0);
}

// Calculates number of WORD arguments before control operators.
// 1. Iterates until pipe, logical or parenthesis token.
// 2. Increments counter for each WORD token.
// 3. Skips two tokens for redirection pairs.
int	get_size_args(t_token **token)
{
	t_token	*t;
	int		i;

	i = 0;
	t = (*token);
	while (t && !is_pipe_or_logical(t))
	{
		if (t->type == WORD)
		{
			t = t->next;
			i++;
		}
		else if (is_redirect(t))
			t = t->next->next;
	}
	return (i);
}

// Checks if string contains single or double quotes.
// 1. Iterates through string.
// 2. Returns 1 if ' or " is found and returns 0 otherwise.
int	have_quotes(char *raw)
{
	int	i;

	i = 0;
	while (raw[i])
	{
		if (raw[i] == '\'' || raw[i] == '\"')
			return (1);
		i++;
	}
	return (0);
}
