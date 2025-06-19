/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wild.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcudos <marcudos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 14:12:14 by marcudos          #+#    #+#             */
/*   Updated: 2025/06/01 18:55:04 by marcudos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Expands wildcards in a linked list of arguments.
// 1. Iterates over each argument.
// 2. If argument contains wildcard, parses pattern and finds matches.
// 3. If no matches, adds argument without quotes.
// 4. If matches found, appends them sorted to the result list.
// 5. Returns the expanded list.
t_args	*expand_wild(t_args *envs)
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
				add_token_free(&result, rem_quotes(envs->arg, 0));
			else
				sort_append(&result, matches);
			free_array((void **) wild.parts);
		}
		else
			add_token_free(&result, rem_quotes(envs->arg, 0));
		envs = envs->next;
	}
	return (result);
}

// Parses a wildcard pattern into parts for matching.
// 1. Checks if argumennt is only asterisks and sets full flag.
// 2. Otherwise splits argument by wildcards into parts.
// 3. Sets flags indicating if pattern has fixed start or end.
// 4. Cleans empty parts if necessary.
// 5. Returns the pattern struct.
t_wild	parse_pattern(char *arg)
{
	t_wild	wild;

	if (only_asterisk(arg))
		return (wild.parts = NULL, wild.full = 1, wild);
	wild.full = 0;
	wild.parts = split_wildcard(arg);
	wild.have_start = 0;
	wild.have_end = 0;
	if (arg && arg[0] != '\0' && arg[0] != '*')
		wild.have_start = 1;
	if (arg && ft_strlen(arg) > 0 && arg[ft_strlen(arg) - 1] != '*')
		wild.have_end = 1;
	if (!wild.parts || count_array((void **)wild.parts) == 0)
	{
		free(wild.parts);
		wild.parts = NULL;
	}
	return (wild);
}

// Finds filenames in the current directory matching the wildcard pattern.
// 1. Opens current directory.
// 2. Iterates over directory entries.
// 3. Skips hidden files unless pattern explicitly starts with '.'.
// 4. Adds matching entries to the result list.
// 5. Returns list of matched filenames.
t_args	*wild_matches(t_wild *wild)
{
	struct dirent	*entry;
	t_args			*matches;
	DIR				*dir;

	matches = NULL;
	dir = opendir(".");
	if (!dir)
		return (NULL);
	entry = readdir(dir);
	while (dir && entry)
	{
		if (entry->d_name[0] == '.'
			&& (!wild->parts || !wild->parts[0] || wild->parts[0][0] != '.'))
			;
		else if (match_pattern(entry->d_name, wild))
			add_token(&matches, entry->d_name);
		entry = readdir(dir);
	}
	closedir(dir);
	return (matches);
}

// Checks if a string contains an unquoted wildcards character.
// 1. Iterates over string characters updating quotes state.
// 2. Returns 1 if unquoted '*' found, else 0.
int	is_wildcard(char *arg)
{
	t_quote	state;
	int		i;

	i = 0;
	state = NO_QUOTE;
	while (arg[i])
	{
		update_state_quote(&state, arg[i]);
		if (arg[i] == '*' && state == NO_QUOTE)
			return (1);
		i++;
	}
	return (0);
}

// Checks if a string consists only of asterisks.
// 1. Iterates through string characters.
// 2. Returns 0 on any non-asterisk character found.
// 3. Returns 1 if all characters are '*'.
int	only_asterisk(char *arg)
{
	int	i;

	i = 0;
	while (arg[i])
	{
		if (arg[i] != '*')
			return (0);
		i++;
	}
	return (1);
}
