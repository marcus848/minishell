/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wild.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcudos <marcudos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 14:12:14 by marcudos          #+#    #+#             */
/*   Updated: 2025/06/01 16:46:02 by marcudos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
				add_token(&result, envs->arg);
			else
				append_list(&result, matches);
			free_array((void **) wild.parts);
		}
		else
			add_token(&result, remove_quotes(envs->arg));
		envs = envs->next;
	}
	return (result);
}

t_wild	parse_pattern(char *arg)
{
	t_wild	wild;
	// int		count;

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
			&& (!wild->parts[0] || wild->parts[0][0] != '.'))
			;
		else if (match_pattern(entry->d_name, wild))
			add_token(&matches, entry->d_name);
		entry = readdir(dir);
	}
	closedir(dir);
	return (matches);
}

int	is_wildcard(char *arg)
{
	t_quote	state;
	int	i;

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
