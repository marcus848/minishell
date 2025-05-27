/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wild.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcudos <marcudos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 14:12:14 by marcudos          #+#    #+#             */
/*   Updated: 2025/05/27 16:19:03 by marcudos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_wildcard(char *arg);
t_wild	parse_pattern(char *arg);
t_args	*wild_matches(t_wild *wild);

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

int	match_middle(char *pos, t_wild *wild, int i, int limit)
{
	size_t	part_len;

	while (i < limit)
	{
		part_len = ft_strlen(wild->parts[i]);
		pos = ft_strnstr(pos, wild->parts[i], ft_strlen(pos));
		if (!pos)
			return (0);
		pos += part_len;
		i++;
	}
	return (1);
}

int	match_end(const char *filename, const char *end)
{
	size_t	file_len;
	size_t	end_len;

	file_len = ft_strlen(filename);
	end_len = ft_strlen(end);
	if (file_len < end_len)
		return (0);
	return (ft_strncmp(filename + file_len - end_len, end, end_len) == 0);
}

int	match_pattern(const char *filename, t_wild *wild)
{
	int	i;
	int	len;
	char	*pos;

	i = 0;
	pos = (char *) filename;
	if (!filename || !wild->parts)
		return (0);
	len = count_array((void **) wild->parts);
	if (wild->have_start)
	{
		if (ft_strncmp(filename, wild->parts[0], ft_strlen(wild->parts[0])) != 0)
			return (0);
		pos = (char *) filename + ft_strlen(wild->parts[0]);
		i++;
	}
	if (!match_middle(pos, wild, i, len - wild->have_end))
		return (0);
	if (wild->have_end)
		return (match_end(filename, wild->parts[len - 1]));
	return (1);
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

