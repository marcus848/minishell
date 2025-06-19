/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wild_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcudos <marcudos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 16:29:40 by marcudos          #+#    #+#             */
/*   Updated: 2025/06/01 18:38:47 by marcudos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Matches all middle parts of a wildcard pattern in a string.
// 1. Iterates from index 'i' to 'limit' in the parts array.
// 2. Uses ft_strnstr to check if each part appears in order in 'pos'.
// 3. Advances the pointer past each match.
// 4. Returns 0 on failure, 1 if all parts match in order.
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

// Checks if a string ends with a giben suffix.
// 1. Gets lengths of filename and suffix.
// 2. Returns 0 if filename is too short.
// 3. Uses ft_strcmp to compare suffix with the end of filename.
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

// Matches a filename against a wildcard pattern.
// 1. Returns true if pattern is a full wildcard (*).
// 2. Handles NULL checks and calculates number of pattern parts.
// 3. If pattern has a start, checks and skips it.
// 4. Verifies all middle parts appear in order.
// 5. If pattern has an end, ensures filename ends with it.
// 6. Returns 1 if pattern matches, 0 otherwise.
int	match_pattern(const char *filename, t_wild *wild)
{
	char	*pos;
	int		i;
	int		len;

	i = 0;
	if (wild->full)
		return (1);
	pos = (char *) filename;
	if (!filename || !wild->parts)
		return (0);
	len = count_array((void **) wild->parts);
	if (wild->have_start)
	{
		if (ft_strncmp(filename, wild->parts[0],
				ft_strlen(wild->parts[0])) != 0)
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

// Handles opening of quotes in input string.
// 1. If single or double quote is found and state is NO_QUOTE, updates state.
// 2. Increments index.
// 3. Returns 1 if a quote was handled, 0 otherwise.
int	handle_opening_quote(char *input, int *i, t_quote *state)
{
	if ((input[*i] == '\'' && *state == NO_QUOTE))
	{
		*state = SINGLE_QUOTE;
		(*i)++;
		return (1);
	}
	else if ((input[*i] == '\"' && *state == NO_QUOTE))
	{
		*state = DOUBLE_QUOTE;
		(*i)++;
		return (1);
	}
	return (0);
}

// Updates quote state while iterating through the input string.
// 1. Handles both opening and closing of quotes.
// 2. Advances index as needed.
// 3. Returns 1 if a quote was handled, 0 otherwise.
int	update_state_quote_i(char *input, int *i, t_quote *state)
{
	int	handled;

	handled = 0;
	while (input[*i] == '\'' || input[*i] == '\"')
	{
		if ((input[*i] == '\'' && *state == SINGLE_QUOTE)
			|| (input[*i] == '\"' && *state == DOUBLE_QUOTE))
		{
			(*i)++;
			*state = NO_QUOTE;
			handled = 1;
		}
		else if (input[*i] == '\'' || input[*i] == '\"')
		{
			if (handle_opening_quote(input, i, state))
				handled = 1;
			else
				break ;
		}
		else
			break ;
	}
	return (handled);
}
