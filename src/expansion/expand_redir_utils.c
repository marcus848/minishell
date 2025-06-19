/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_redir_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caide-so <caide-so@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 14:01:51 by caide-so          #+#    #+#             */
/*   Updated: 2025/06/15 14:03:23 by caide-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Counts the number of elements in a linked list of arguments.
// 1. Iterates through the list.
// 2. Increments count for each node.
// 3. Returns the total count.
int	count_args(t_args *list)
{
	int	count;

	count = 0;
	while (list)
	{
		count++;
		list = list->next;
	}
	return (count);
}

// Handles the case when no matches are found for a redirection filename.
// 1. Sets shell last_status to 1.
// 2. Prints error message with filename.
// 3. Sets redirection filename to NULL to indicate failure.
void	handle_no_match(t_shell *sh, t_redir *redir)
{
	sh->last_status = 1;
	ft_putstr_fd("-minishell: ", STDERR_FILENO);
	ft_putstr_fd(redir->filename, STDERR_FILENO);
	ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
	redir->filename = NULL;
}

// Handles the case when a redirection filename expansion is ambigous
// (multiple matches).
// 1. Sets shell last_status to 1.
// 2. Prints ambiguous redirect error with filename.
// 3. Sets redirection filename to NULL to indicate failure.
void	handle_ambiguous(t_shell *sh, t_redir *redir)
{
	sh->last_status = 1;
	ft_putstr_fd("-minishell: ", STDERR_FILENO);
	ft_putstr_fd(redir->filename, STDERR_FILENO);
	ft_putstr_fd(": ambiguous redirect\n", STDERR_FILENO);
	redir->filename = NULL;
}

// Handle the case when a redirection filename expands to a single match.
// 1. Frees old filename string.
// 2. Replaces it with the expanded filename string.
void	handle_single_match(t_redir *redir, t_args *expanded_list)
{
	char	*old_filename;

	old_filename = redir->filename;
	redir->filename = ft_strdup(expanded_list->arg);
	free(old_filename);
}
