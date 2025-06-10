/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caide-so <caide-so@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 00:23:33 by caide-so          #+#    #+#             */
/*   Updated: 2025/06/10 00:33:39 by caide-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	append_redir(t_redir **head, char *filename);
void	append_heredoc(t_heredoc **head, t_heredoc *new_hd);

void	parse_redir(t_token **token, t_redir **redir)
{
	append_redir(redir, (*token)->next->value);
	*token = (*token)->next->next;
}

void	parse_infile(t_token **token, t_command **command)
{
	append_redir(&(*command)->infiles, (*token)->next->value);
	*token = (*token)->next->next;
}

void	append_redir(t_redir **head, char *filename)
{
	t_redir	*redir;
	t_redir	*iter;

	redir = (t_redir *)malloc(sizeof(*redir));
	if (!redir)
		exit_perror("malloc");
	redir->filename = rem_quotes(ft_strdup(filename), 1);
	redir->next = NULL;
	if (!*head)
		*head = redir;
	else
	{
		iter = *head;
		while (iter->next)
			iter = iter->next;
		iter->next = redir;
	}
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
