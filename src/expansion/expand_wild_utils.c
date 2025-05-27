/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wild_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcudos <marcudos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 16:29:40 by marcudos          #+#    #+#             */
/*   Updated: 2025/05/27 16:29:55 by marcudos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
