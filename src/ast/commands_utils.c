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

t_redir	*create_heredoc_node(char *raw, t_shell *shell);
char	*strip_quotes(char *raw, int *no_expand);
void	build_redir_list(t_redir **head, t_redir *node);

// Parses and adds a heredoc redirection to the command.
// 1. Creates a heredoc node if not interrupted by signal.
// 2. Appends it to the command's redirection list.
// 3. Advances token pointer by two (operator + delimiter).
// Note: Relies on global g_signal_status to skip heredoc on interrupt.
void	parse_heredoc(t_token **token, t_command **cmd, t_shell *shell)
{
	t_redir	*redir;

	if (g_signal_status == -1)
	{
		redir = create_heredoc_node((*token)->next->value, shell);
		build_redir_list(&(*cmd)->redirs, redir);
	}
	(*token) = (*token)->next->next;
}

// Creates a heredoc redirection node.
// 1. Extracts delimiter with or without quote expansion.
// 2. Processes heredoc input and gets temporary FD.
// 3. Allocates and fills t_redir node with type R_HEREDOC.
// 4. Sets filename, heredoc_fd and next pointer.
t_redir	*create_heredoc_node(char *raw, t_shell *shell)
{
	int		no_expand;
	char	*delim;
	int		fd;
	t_redir	*redir;

	delim = strip_quotes(raw, &no_expand);
	fd = process_hd(delim, no_expand, shell->env, shell);
	redir = (t_redir *)malloc(sizeof(*redir));
	if (!redir)
		exit_perror("malloc");
	redir->type = R_HEREDOC;
	redir->filename = delim;
	redir->heredoc_fd = fd;
	redir->next = NULL;
	return (redir);
}

// Removes quotes form heredoc delimiter.
// 1. Sets no_expand flag if quotes are present.
// 2. Returns duplicated string with quotes removed.
char	*strip_quotes(char *raw, int *no_expand)
{
	*no_expand = have_quotes(raw);
	return (rem_quotes(ft_strdup(raw), 1));
}

// Appends a redirection node to the redir list.
// 1. If list is empty, sets head to node.
// 2. Otherwise, traverses to end and appends node.
void	build_redir_list(t_redir **head, t_redir *node)
{
	t_redir	*iter;

	if (!*head)
		*head = node;
	else
	{
		iter = *head;
		while (iter->next)
			iter = iter->next;
		iter->next = node;
	}
}

// Creates and appends a redirection node (non-heredoc).
// 1. Duplicates and strips quotes from a filename.
// 2. Initializes t_redir with given type and filename.
// 3. Sets heredoc_fd to -1 and next to NULL.
// 4. Appends to redirection list.
// Note: Used for <, > and >> redirections.
void	append_redir(t_redir **head, t_redir_type type, char *filename)
{
	t_redir	*redir;

	redir = (t_redir *)malloc(sizeof(*redir));
	if (!redir)
		exit_perror("malloc");
	redir->type = type;
	redir->filename = rem_quotes(ft_strdup(filename), 1);
	redir->heredoc_fd = -1;
	redir->next = NULL;
	build_redir_list(head, redir);
}
