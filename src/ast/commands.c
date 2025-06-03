/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcudos <marcudos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 19:58:35 by marcudos          #+#    #+#             */
/*   Updated: 2025/05/29 00:08:38 by caide-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	append_heredoc(t_heredoc **head, t_heredoc *new_hd);

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

void	parse_redirect(t_token **token, t_command **command)
{
	if (!(*token)->next)
	{
		(*token) = (*token)->next;
		return ;
	}
	if ((*token)->type == REDIR_IN)
	{
		(*command)->infile = ft_strdup((*token)->next->value);
		(*token) = (*token)->next->next;
	}
	else if ((*token)->type == REDIR_OUT)
	{
		(*command)->outfile = ft_strdup((*token)->next->value);
		(*token) = (*token)->next->next;
	}
	else if ((*token)->type == REDIR_APPEND)
	{
		(*command)->appendfile = ft_strdup((*token)->next->value);
		(*token) = (*token)->next->next;
	}
	else if ((*token)->type == HEREDOC)
		parse_heredoc(token, command);
}

void	parse_heredoc(t_token **token, t_command **command)
{
	char		*raw;
	size_t		len;
	t_heredoc	*new_hd;

	raw = (*token)->next->value;
	len = ft_strlen(raw);
	new_hd = (t_heredoc *)malloc(sizeof(t_heredoc));
	if (!new_hd)
		exit_perror("malloc");
	if ((raw[0] == '\'' && raw[len - 1] == '\'')
		|| (raw[0] == '"' && raw[len - 1] == '"'))
	{
		new_hd->quoted_delim = 1;
		new_hd->delimiter = ft_substr(raw, 1, len - 2);
	}
	else
	{
		new_hd->quoted_delim = 0;
		new_hd->delimiter = ft_strdup(raw);
	}
	new_hd->next = NULL;
	append_heredoc(&(*command)->heredocs, new_hd);
	(*token) = (*token)->next->next;
}

void	append_heredoc(t_heredoc **head, t_heredoc *new_hd)
{
	t_heredoc	*iter;

	if (!*head)
		*head = new_hd;
	else
	{
		iter = *head;
		while (iter->next)
			iter = iter->next;
		iter->next = new_hd;
	}
}

t_command	*init_command(void)
{
	t_command	*command;

	command = (t_command *) malloc(sizeof(t_command));
	if (!command)
		return (NULL);
	command->args = NULL;
	command->arg_count = 0;
	command->infile = NULL;
	command->outfile = NULL;
	command->appendfile = NULL;
	command->heredocs = NULL;
	command->heredoc_fd = -1;
	command->is_builtin = 0;
	return (command);
}
//
// char	**get_args(t_token **token, int size_args)
// {
// 	char	**args;
// 	int		i;
//
// 	args = (char **) malloc(sizeof(char *) * (size_args + 1));
// 	i = 0;
// 	if (!args)
// 		return (NULL);
// 	while (i < size_args)
// 	{
// 		args[i] = ft_strdup((*token)->value);
// 		(*token) = (*token)->next;
// 		i++;
// 	}
// 	args[i] = NULL;
// 	return (args);
// }
