/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcudos <marcudos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 19:58:35 by marcudos          #+#    #+#             */
/*   Updated: 2025/06/10 00:36:55 by caide-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_command	*make_command(t_token **token)
{
	t_command	*command;
	t_token		*t;
	int			arg;

	command = init_command();
	if (!command)
		return (NULL);
	command->arg_count = get_size_args(token);
	command->args = (char **) malloc(sizeof(char *) * (command->arg_count + 1));
	if (!command->args)
		return (NULL);
	t = *token;
	arg = 0;
	while (t && !is_pipe_or_logical(t))
	{
		if (t->type == WORD)
		{
			command->args[arg++] = ft_strdup(t->value);
			t = t->next;
		}
		else if (is_redirect(t))
			parse_redirect(&t, &command);
	}
	command->args[arg] = NULL;
	return ((*token) = t, command);
}

t_command	*init_command(void)
{
	t_command	*command;

	command = (t_command *) malloc(sizeof(t_command));
	if (!command)
		return (NULL);
	command->args = NULL;
	command->arg_count = 0;
	command->infiles = NULL;
	command->outfile = NULL;
	command->appendfile = NULL;
	command->heredocs = NULL;
	command->heredoc_fd = -1;
	command->is_builtin = 0;
	return (command);
}

void	parse_redirect(t_token **token, t_command **command)
{
	if (!(*token)->next)
	{
		*token = (*token)->next;
		return ;
	}
	if ((*token)->type == REDIR_IN)
		parse_redir(token, &(*command)->infiles);
	else if ((*token)->type == REDIR_OUT)
	{
		(*command)->outfile = rem_quotes(ft_strdup((*token)->next->value), 1);
		(*token) = (*token)->next->next;
	}
	else if ((*token)->type == REDIR_APPEND)
	{
		(*command)->appendfile = rem_quotes(
				ft_strdup((*token)->next->value), 1);
		(*token) = (*token)->next->next;
	}
	else if ((*token)->type == HEREDOC)
		parse_heredoc(token, command);
}
