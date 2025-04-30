/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcudos <marcudos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 16:50:15 by marcudos          #+#    #+#             */
/*   Updated: 2025/04/30 18:01:18 by marcudos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	size_args(t_token **token);
t_ast	*make_node_command(t_token **token);
t_ast	*start_ast(t_token **token);

t_ast	*make_ast(t_token_list *tokens)
{
	t_ast	*top;
	t_ast	*new;
	t_token		*token;

	token = tokens->head;
	top = start_ast(&token);
	while (token && token->type == TOKEN_LOGICAL_AND)
	{
		token = token->next;
		new = malloc(sizeof(t_ast));
		if (!new)
			return (NULL);
		new->type = NODE_AND;
		new->left = top;
		new->right = make_node_command(&token);
		new->args = NULL;
		top = new;
	}
	return (top);
}


t_ast	*start_ast(t_token **token)
{
	t_ast	*top;
	t_ast	*left;

	left = make_node_command(token);
	if (!*token)
		return (left);
	if ((*token)->type != TOKEN_PIPE &&
		(*token)->type != TOKEN_LOGICAL_OR &&
		(*token)->type != TOKEN_LOGICAL_AND)
		return (left);
	top = malloc(sizeof(t_ast));
	if (!top)
		return (NULL);
	top->left = left;
	top->args = NULL;
	top->right = NULL;
	if ((*token)->type == TOKEN_PIPE)
		top->type = NODE_PIPE;
	else if ((*token)->type == TOKEN_LOGICAL_OR)
		top->type = NODE_OR;
	else if ((*token)->type == TOKEN_LOGICAL_AND)
		top->type = NODE_AND;
	(*token) = (*token)->next;
	top->right = make_node_command(token);
	return (top);
}


t_ast	*make_node_command(t_token **token)
{
	t_ast	*new;
	char		**args;
	int			s_args;
	int			i;

	i = 0;
	s_args = size_args(token);
	args = malloc(sizeof(char *) * (s_args + 1));
	new = malloc(sizeof(t_ast));
	if (!args || !new)
		return (NULL);
	while (i < s_args)
	{
		args[i] = ft_strdup((*token)->value);
		(*token) = (*token)->next;
		i++;
	}
	args[i] = NULL;
	new->type = NODE_COMMAND;
	new->args = args;
	new->left = NULL;
	new->right = NULL;
	return (new);
}

int	size_args(t_token **token)
{
	t_token	*t;
	int	i;

	i = 0;
	t = (*token);
	while (t && t->type == TOKEN_WORD)
	{
		i++;
		t = t->next;
	}
	return (i);
}
