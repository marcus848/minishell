/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wild.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcudos <marcudos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 14:12:14 by marcudos          #+#    #+#             */
/*   Updated: 2025/05/27 16:32:04 by marcudos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_args	*expand_wild(t_args *envs)
{
	t_args	*result;
	t_args	*current;
	t_args	*matches;
	t_wild	wild;

	result = NULL;
	current = envs;
	while (current)
	{
		if (is_wildcard(current->arg))
		{
			wild = parse_pattern(current->arg);
			matches = wild_matches(&wild);
			if (!matches)
				add_token(&result, current->arg);
			else
				append_list(&result, matches);
			free_array((void **) wild.parts);
		}
		else
			add_token(&result, current->arg);
		current = current->next;
	}
	return (result);
}

t_wild	parse_pattern(char *arg)
{
	t_wild	wild;
	int	count;

	wild.parts = ft_split(arg, '*');
	wild.have_start = 0;
	wild.have_end = 0;
	if (arg && arg[0] != '\0' && arg[0] != '*')
		wild.have_start = 1;
	if (arg && ft_strlen(arg) > 0 && arg[ft_strlen(arg) - 1] != '*')
		wild.have_end = 1;
	count = count_array((void **) wild.parts);
	if (count == 0)
	{
		free(wild.parts);
		wild.parts = NULL;
	}
	return (wild);
}

t_args	*wild_matches(t_wild *wild)
{
	t_args	*matches;
	DIR	*dir;
	struct dirent	*entry;

	matches = NULL;
	dir = opendir(".");
	if (!dir)
		return (NULL);
	while (dir && (entry = readdir(dir)))
	{
		if (entry->d_name[0] == '.'
			&& (!wild->parts[0] || wild->parts[0][0] != '.'))
			continue ;		
		if (match_pattern(entry->d_name, wild))
			add_token(&matches, entry->d_name);
	}
	closedir(dir);
	return (matches);
}

int	is_wildcard(char *arg)
{
	int	i;

	i = 0;
	while (arg[i])
	{
		if (arg[i] == '*')
			return (1);
		i++;
	}
	return (0);
}

