/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcudos <marcudos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 19:58:35 by marcudos          #+#    #+#             */
/*   Updated: 2025/04/30 20:32:20 by marcudos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <stdlib.h>

t_command	*make_command(t_token **token)
{
	t_command	*command;
	t_token		*t;

	command = init_command();
	command->arg_count = get_size_args(token);
	command->args = get_args(token, command->arg_count);
	while (t->type != LOGICAL_OR && t->type != LOGICAL_AND && t->type != PIPE
		&& t->type != PAREN_OPEN && t->type != PAREN_CLOSE)
	{
		if (t->type == REDIR_IN)
		{
		}
	}
	return (command);
}

t_command	*init_command(void)
{
	t_command	*command;

	command = (t_command *) malloc(sizeof(t_command));
	command->args = NULL;
	command->arg_count = 0;
	command->infile = NULL;
	command->outfile = NULL;
	command->appendfile = NULL;
	command->heredoc = 0;
	command->delimiter = NULL;
	command->heredoc_path = NULL;
	command->is_builtin = 0;
	return (command);
}

char	**get_args(t_token **token, int size_args)
{
	char	**args;
	int		i;

	args = (char **) malloc(sizeof(char *) * (size_args + 1));
	i = 0;
	if (!args)
		return (NULL);
	while (i < size_args)
	{
		args[i] = ft_strdup((*token)->value);
		(*token) = (*token)->next;
		i++;
	}
	args[i] = NULL;
	return (args);
}

int	get_size_args(t_token **token)
{
	t_token	*t;
	int	i;

	i = 0;
	t = (*token);
	while (t && t->type == WORD)
	{
		i++;
		t = t->next;
	}
	return (i);
}
