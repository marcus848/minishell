/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caide-so <caide-so@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 13:24:54 by caide-so          #+#    #+#             */
/*   Updated: 2025/06/15 14:04:48 by caide-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Expands all redirections except heredocs.
// 1. Iterates through all redirections.
// 2. Calls expand_redir on each except type R_HEREDOC.
void	expand_redirs(t_redir *redirs, t_shell *sh)
{
	t_redir	*cur;

	cur = redirs;
	while (cur)
	{
		if (cur->type != R_HEREDOC)
			expand_redir(cur, sh);
		cur = cur->next;
	}
}

// Expands wildcards in a single redirection's filename.
// 1. Converts filename into argument list.
// 2. Expands wildcards in the argument list.
// 3. Counts matches and handles:
// 	- No match: calls handle_no_match().
// 	- Multiple matches: calls handle_ambigous().
// 	- Single match: calls handle_single_match().
// 4. Frees temporary argument lists.
void	expand_redir(t_redir *redir, t_shell *sh)
{
	t_args	*arg_list;
	t_args	*expanded_list;
	int		count;

	arg_list = NULL;
	add_token(&arg_list, redir->filename);
	expanded_list = expand_redir_wild(arg_list);
	if (!expanded_list)
		count = 0;
	else
		count = count_args(expanded_list);
	if (count == 0)
		handle_no_match(sh, redir);
	else if (count > 1)
		handle_ambiguous(sh, redir);
	else
		handle_single_match(redir, expanded_list);
	free_args_list(arg_list);
	free_args_list(expanded_list);
}

// Expands wildcards in an argument list.
// 1. Iterates over args and detects wildcards.
// 2. Parses wildcards patterns and finds matches.
// 3. Sorts and appends matches to result list.
// 4. Removes quotes from non-wildcard args and adds them directly.
// 5. Returns the expanded argument list or NULL on failure.
t_args	*expand_redir_wild(t_args *envs)
{
	t_args	*result;
	t_args	*matches;
	t_wild	wild;

	result = NULL;
	while (envs)
	{
		if (is_wildcard(envs->arg))
		{
			wild = parse_pattern(envs->arg);
			matches = wild_matches(&wild);
			if (!matches)
			{
				free_array((void **)wild.parts);
				free_args_list(result);
				return (NULL);
			}
			sort_append(&result, matches);
			free_array((void **)wild.parts);
		}
		else
			add_token_free(&result, rem_quotes(envs->arg, 0));
		envs = envs->next;
	}
	return (result);
}
