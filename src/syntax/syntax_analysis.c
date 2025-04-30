/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_analysis.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caide-so <caide-so@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 19:22:39 by caide-so          #+#    #+#             */
/*   Updated: 2025/04/30 20:31:17 by caide-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	token_type_analysis(t_token *prev, t_token *token, t_token *next);
int	check_pipe(t_token *prev, t_token *token, t_token *next);
int	check_redir(t_token *prev, t_token *token, t_token *next);
int	check_logical(t_token *prev, t_token *token, t_token *next);
int	check_paren(t_token *prev, t_token *token, t_token *next);

int	syntax_analysis(t_token_list *tokens)
{
	t_token	*curr;
	t_token	*prev;

	if (tokens->size == 0)
		return (0);
	curr = tokens->head;
	prev = NULL;
	if (curr->type != WORD && curr->type != PAREN_OPEN)
	{
		report_syntax_error(ERR_PAREN);
		return (0);
	}
	while (curr)
	{
		if (!token_type_analysis(prev, curr, curr->next))
			return (0);
		prev = curr;
		curr = curr->next;
	}
	if (prev->type != WORD && prev->type != PAREN_CLOSE)
	{
		report_syntax_error(ERR_PAREN);
		return (0);
	}
	return (1);
}

int	token_type_analysis(t_token *prev, t_token *token, t_token *next)
{
	if (token->type == PIPE)
		return (check_pipe(prev, token, next));
	else if (token->type == REDIR_IN || token->type == REDIR_OUT
		|| token->type == REDIR_APPEND || token->type == HEREDOC)
		return (!check_redir(prev, token, next));
	else if (token->type == LOGICAL_AND || token->type == LOGICAL_OR)
		return (!check_logical(prev, token, next));
	else if (token->type == PAREN_OPEN || token->type == PAREN_CLOSE)
		return (!check_paren(prev, token, next));
	return (1);
}
