/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_analysis.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caide-so <caide-so@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 19:22:39 by caide-so          #+#    #+#             */
/*   Updated: 2025/05/02 15:06:11 by caide-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	token_analysis(t_token *prev, t_token *token, t_token *next, int *depth);

int	syntax_analysis(t_token_list *tokens)
{
	t_token	*curr;
	t_token	*prev;
	int		depth;

	depth = 0;
	if (tokens->size == 0)
		return (report_unexpected("newline"));
	curr = tokens->head;
	prev = NULL;
	if (curr->type == PIPE || curr->type == LOGICAL_AND
		|| curr->type == LOGICAL_OR)
		return (report_unexpected(curr->value));
	while (curr)
	{
		if (!token_analysis(prev, curr, curr->next, &depth))
			return (0);
		prev = curr;
		curr = curr->next;
	}
	if (prev->type != WORD && prev->type != PAREN_CLOSE)
		return (report_unexpected("newline"));
	if (depth != 0)
		return (report_unexpected("newline"));
	return (1);
}

int	token_analysis(t_token *prev, t_token *token, t_token *next, int *depth)
{
	if (token->type == REDIR_IN || token->type == REDIR_OUT
		|| token->type == REDIR_APPEND || token->type == HEREDOC)
		return (check_redir(next));
	else if (token->type == PIPE)
		return (check_pipe(prev, next));
	else if (token->type == LOGICAL_AND || token->type == LOGICAL_OR)
		return (check_logical(prev, token, next));
	else if (token->type == PAREN_OPEN || token->type == PAREN_CLOSE)
		return (check_paren(prev, token, next, depth));
	return (1);
}

int	is_all_whitespace(const char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] != ' ' && s[i] != '\t')
			return (0);
		i++;
	}
	return (1);
}
