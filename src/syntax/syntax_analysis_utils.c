/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_analysis_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caide-so <caide-so@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 14:26:12 by caide-so          #+#    #+#             */
/*   Updated: 2025/05/02 14:32:23 by caide-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Validates pipe token placement.
// 1. Checks previous token is WORD or PAREN_CLOSE.
// 2. Checks next token is WORD, PAREN_OPEN, or any redirection.
// Returns 1 if valid, 0 on error.
int	check_pipe(t_token *prev, t_token *next)
{
	if (!prev || (prev->type != WORD && prev->type != PAREN_CLOSE))
		return (report_unexpected("|"));
	if (!next || (next->type != WORD && next->type != PAREN_OPEN
			&& next->type != REDIR_IN && next->type != REDIR_OUT
			&& next->type != REDIR_APPEND && next->type != HEREDOC))
		return (report_unexpected("|"));
	return (1);
}

// Validates redirection token placement.
// 1. Ensures redirection is followed by WORD.
// Returns 1 if valid, 0 on error.
int	check_redir(t_token *next)
{
	const char	*off;

	if (next)
		off = next->value;
	else
		off = "newline";
	if (!next || next->type != WORD)
		return (report_unexpected(off));
	return (1);
}

// Validates logical operator placement.
// 1. Checks previous token is WORD or PAREN_CLOSE.
// 2. Ensures next token isn't another operator.
// Returns 1 if valid, 0 on error.
int	check_logical(t_token *prev, t_token *token, t_token *next)
{
	const char	*sym;

	if (is_twochar(token) && token->type == LOGICAL_OR)
		sym = "||";
	else
		sym = "&&";
	if (!prev || (prev->type != WORD && prev->type != PAREN_CLOSE))
		return (report_unexpected(sym));
	if (next && (next->type == PIPE || next->type == LOGICAL_AND
			|| next->type == LOGICAL_OR || next->type == PAREN_CLOSE))
		return (report_unexpected(next->value));
	return (1);
}

// Checks if token is a two-character operator.
// Returns 1 for logical/redirection operators, 0 otherwise.
int	is_twochar(t_token *token)
{
	return (token->type == LOGICAL_AND || token->type == LOGICAL_OR
		|| token->type == REDIR_APPEND || token->type == HEREDOC);
}

// Validates parenthesis placement and nesting.
// 1. Checks opening parenthesis has valid content.
// 2. Validates closing parenthesis position.
// 3. Tracks nesting depth.
// Returns 1 if valid, 0 on error.
int	check_paren(t_token *prev, t_token *token, t_token *next, int *depth)
{
	if (token->type == PAREN_OPEN)
	{
		if (next && next->type == PAREN_CLOSE)
			return (report_unexpected(")"));
		if (!next || (next->type != WORD && next->type != PAREN_OPEN))
			return (report_unexpected("("));
		(*depth)++;
	}
	else
	{
		if (!prev || (prev->type != WORD && prev->type != PAREN_CLOSE))
			return (report_unexpected(")"));
		(*depth)--;
		if (*depth < 0)
			return (report_unexpected(")"));
	}
	return (1);
}
